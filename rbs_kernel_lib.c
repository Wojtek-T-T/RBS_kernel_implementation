#include "rbs_lib.h"

struct timespec time_reference;
struct log_event_data *log_event_buffers_ptrs[400];
u_int32_t buff_indexes[400];


void InitializeSequence(struct task_data *taskDATA, int sequenceID, pthread_t *thread, pthread_attr_t attr, void *(*func)())
{
	//Allocate memory for sequence data
	struct sequence_data *sequenceDATA = malloc(sizeof(struct sequence_data));
	
	sequenceDATA->task = taskDATA;
	sequenceDATA->sequence_id = sequenceID;
	sequenceDATA->current_job = taskDATA->last_added_job;
	
	
	//Initialize thread
    int result = pthread_create(thread, &attr, func, (void*) sequenceDATA);
 
}

void initialize_rbs()
{
    clock_gettime(CLOCK_REALTIME, &time_reference);
    result = syscall(INIT);

}

int InitializeTask(struct task_data *taskDATA)
{
    int result = 0;

    if((taskDATA->priority > 99) || (taskDATA->priority < 1))
    {
        //Priority out of range
        return 1;
    }

    //Allocate memory for previous job, this structure is needed only for purpose of correct functioning of the double linked list
    taskDATA->last_added_job = malloc(sizeof(struct job_token));
    taskDATA->last_added_job->secondary_sequences_guards = NULL;

    //SET SCHEDULER TO SCHED_FIFO and set priority of the task
    pthread_attr_init(&taskDATA->attr);
    result = pthread_attr_setschedpolicy(&taskDATA->attr, SCHED_FIFO);
    taskDATA->schedPARAM.sched_priority = taskDATA->priority;
    result = pthread_attr_setschedparam(&taskDATA->attr, &taskDATA->schedPARAM);
    result = pthread_attr_setinheritsched(&taskDATA->attr, PTHREAD_EXPLICIT_SCHED);

    //Initialize log buffer
	#ifdef LOG_DATA
        for(int buff_nr = 0; buff_nr <= taskDATA->number_of_sequences; buff_nr ++)
        {
            int index = (taskDATA->task_id - 1) * 20 + buff_nr;
            if(buff_nr == 0)
            {
                log_event_buffers_ptrs[index] = calloc(10, sizeof(struct log_event_data));
            }
            else
            {
                log_event_buffers_ptrs[index] = calloc(10000, sizeof(struct log_event_data));
            }
            buff_indexes[index] = 0;
        }
	#endif

    result = syscall(INIT_TASK, taskDATA->task_id, taskDATA->number_of__nodes, taskDATA->number_of_sequences);
    result = syscall(TRANSFER, taskDATA->task_id, taskDATA->pre_cons_v, taskDATA->pre_cons_h, taskDATA->sequence_heads);

    return 0;
}

struct log_event_data *log_event_start(int task, int sequence, int node, int job, int event)
{
    int index = (task - 1) * 20 + sequence;
    int local_buff_index = buff_indexes[index];

    struct log_event_data *ptr = log_event_buffers_ptrs[index] + local_buff_index;

    clock_gettime(CLOCK_REALTIME, &ptr->start_time);

    ptr->task = task;
    ptr->sequence = sequence;
    ptr->node = node;
    ptr->job = job;
    ptr->event = event;

    buff_indexes[index] = buff_indexes[index] + 1;

    return ptr;
}

void log_event_end(struct log_event_data *ptr)
{
    clock_gettime(CLOCK_REALTIME, &ptr->end_time);
}

void log_first_activation(int task, struct timespec activation)
{
    int index = (task - 1) * 20;
    int local_buff_index = buff_indexes[index];

    struct log_event_data *ptr = log_event_buffers_ptrs[index] + local_buff_index;

    ptr->task = task;
    ptr->sequence = 0;
    ptr->node = 0;
    ptr->job = 0;
    ptr->event = START_TIME_REL_FUNC;
    ptr->start_time.tv_sec = activation.tv_sec;
    ptr->start_time.tv_nsec = activation.tv_nsec;

    buff_indexes[index] = buff_indexes[index] + 1;
}

void set_cpu(int cpu_num)
{
   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(cpu_num, &cpuset);  
   sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}

void WaitNextJob(struct sequence_data *sequenceDATA)
{
	//Wait till a new job is released
    int ret = syscall(WAIT, sequenceDATA->task->task_id, sequenceDATA->sequence_id);

}

void ReleaseNewJob(struct task_data *taskDATA)
{

    //Increase the jobs counter
	taskDATA->job_counter = taskDATA->job_counter + 1;
	
    int ret = syscall(RELEASE, taskDATA->task_id);
}

void FinishJob(struct sequence_data *sequenceDATA)
{
        return;

}



int TryExecuteNode(struct sequence_data *sequenceDATA, int node)
{
    
    int ret = syscall(EXECUTE, sequenceDATA->task->task_id, sequenceDATA->sequence_id, node);
    
    //Log event
    #ifdef LOG_DATA
        struct log_event_data *log_ptr = log_event_start(sequenceDATA->task->task_id, sequenceDATA->sequence_id, node, sequenceDATA->current_job->job_id, NODE_EXECUTION);
    #endif
    
    //Execute Node function
    sequenceDATA->task->func[(node-1)]();

    ret = syscall(EXECUTED, sequenceDATA->task->task_id, sequenceDATA->sequence_id, node);

    //Log event
    #ifdef LOG_DATA
        log_event_end(log_ptr);
    #endif
    
    
    return 0;
}


void TerminateSequence(struct sequence_data *sequenceDATA, int node)
{
    #ifdef LOG_DATA
    //log_event_start(sequenceDATA->task->task_id, sequenceDATA->sequence_id, node, sequenceDATA->current_job->job_id, SEQUENCE_TERMINATED);
    #endif
}


void print_log_data_json(struct task_data *taskDATA_start, int num_of_tasks)
{
    FILE *fp;
    fp = fopen("log.json", "w");
    fprintf(fp, "{\"log\":[\n");

    for(int task = 0; task < num_of_tasks; task ++)
    {
        struct task_data *taskDATA = taskDATA_start + task;

        for(int i = 0 ; i <= taskDATA->number_of_sequences; i ++)
        {
            int index = (taskDATA->task_id - 1) * 20 + i;

            for(int x = 0; x < buff_indexes[index]; x++)
            {
                
                struct log_event_data *ptr = log_event_buffers_ptrs[index] + x;

                struct timespec start_time_loc;
                struct timespec end_time_loc;

                start_time_loc.tv_sec = ptr->start_time.tv_sec - time_reference.tv_sec;
                start_time_loc.tv_nsec = ptr->start_time.tv_nsec; 

                double start_time_stamp = (double)start_time_loc.tv_sec * 1000000;
                start_time_stamp = start_time_stamp + (start_time_loc.tv_nsec/1000);

                end_time_loc.tv_sec = ptr->end_time.tv_sec - time_reference.tv_sec;
                end_time_loc.tv_nsec = ptr->end_time.tv_nsec; 

                double time_stamp_end = (double)end_time_loc.tv_sec * 1000000;
                time_stamp_end = time_stamp_end + (end_time_loc.tv_nsec/1000);



                int task = ptr->task;
                int sequence = ptr->sequence;
                int node = ptr->node;
                int job = ptr->job;
                int event = ptr->event;

                if(task == (num_of_tasks) && x == (buff_indexes[index] - 1) && i == taskDATA->number_of_sequences)
                {
                    fprintf(fp, "{\"type\" : %d, \"task\" : %d, \"sequence\" : %d, \"node\" : %d, \"job\" : %d, \"start\" : %f, \"end\" : %f}\n",event, task, sequence, node, job, start_time_stamp, time_stamp_end);
                }
                else
                {
                    fprintf(fp, "{\"type\" : %d, \"task\" : %d, \"sequence\" : %d, \"node\" : %d, \"job\" : %d, \"start\" : %f, \"end\" : %f},\n",event, task, sequence, node, job, start_time_stamp, time_stamp_end);
                
                }

            
            }
        }
    }
    fprintf(fp, "]\n");
    fprintf(fp, "}\n");
    fclose(fp);

}
