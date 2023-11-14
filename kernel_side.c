//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// NEW SYSTEM CALLS ADDED AS A PART OF RBS PROJECT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
int value2 = 67;
struct semaphore my_first_kernel_sem;

struct task_data *tasks[100];

bool rbs_check_precedence_constraints(int task_id, int sequence_id, int node_id)
{
    //source nodes doesn't have any precedence constraints
    if(node_id == 1)
    {
        return true;
    }
    
    u_int32_t *ptr = tasks[task_id]->kernel_pre_h + (node_id -1);
    if(*ptr == (*ptr & tasks[task_id]->current_sequence_jobs[sequence_id-1]->nodes_finished))
    {
        return true;
    }
    else
    {
        return false;
    } 

}

bool rbs_check_if_node_in_execution(int task_id, int sequence_id, int node_id)
{
    u_int32_t mask = 1;
    mask = mask << (node_id - 1);
    u_int32_t local_execution_state = tasks[task_id]->current_sequence_jobs[sequence_id-1]->nodes_in_execution;
    
    local_execution_state = local_execution_state & mask;
    
    if(local_execution_state > 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void rbs_mark_node_in_execution(int task_id, int sequence_id, int node_id)
{
    u_int32_t mask = 1;
    mask = mask << (node_id - 1);

	tasks[task_id]->current_sequence_jobs[sequence_id-1]->nodes_in_execution = tasks[task_id]->current_sequence_jobs[sequence_id-1]->nodes_in_execution | mask;
}


void rbs_mark_node_executed(int task_id, int sequence_id, int node_id)
{
    //Mark task as finished by setting the bit in the job state variable
    u_int32_t mask = 1;
    mask = mask << (node_id - 1);
    tasks[task_id]->current_sequence_jobs[sequence_id-1]->nodes_finished = tasks[task_id]->current_sequence_jobs[sequence_id-1]->nodes_finished | mask;
	return;
}

void rbs_signal_sequence(int task_id, int sequence_id, int node_id)
{
    //Nodes that have incoming precedence constraints from the finished node
    u_int32_t pre_cons = *(tasks[task_id]->kernel_pre_v + node_id - 1);

    //If there are no nodes with incoming precedence constraints from the finshed node => nothing to signal
    if(pre_cons == 0)
    {
        return;
    }

    for(int i = 0; i < tasks[task_id]->number_of_nodes; i ++)
    {
        u_int32_t mask = 1 << i;

        if((mask & pre_cons) != 0)
        {
            //number of sequence of which the node is head of
            int sequence = *(tasks[task_id]->kernel_sequence_heads + i);

            //if 0 than the node is not a head of any sequence
            if(sequence != 0)
            {
                if(rbs_check_precedence_constraints(task_id, sequence_id, node_id))
                {
                    //SIGNAL
					struct semaphore *guard_to_signal = tasks[task_id]->current_sequence_jobs[sequence_id - 1]->sec_guards + sequence_id -1;
                    up(guard_to_signal);
                }
            }
        }
    }


	return;
}

void rbs_finish_job(int task_id, int sequence_id, int node_id)
{
    //If object is empty (first token is always empty) do nothing
    if(tasks[task_id]->current_sequence_jobs[sequence_id - 1]->sec_guards == NULL)
    {
        return;
    }

    //Destroy mutex
   // pthread_mutex_destroy(&sequenceDATA->current_job->previous_job->job_lock); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //Destroy secondary semaphores
    //for(int i = 0; i < tasks[task_id]->number_of_sequences; i++)
    //{
        //sem_destroy((sequenceDATA->current_job->previous_job->secondary_sequences_guards + i));
    //}
    
    //Free memory
    kfree(tasks[task_id]->current_sequence_jobs[sequence_id - 1]->previous_job->sec_guards);
    kfree(tasks[task_id]->current_sequence_jobs[sequence_id - 1]->previous_job);	
}


SYSCALL_DEFINE2(hellotest, int, num1, int, num2)
{
	/*
		struct job_token token;
		token.value = 5;
        printk("HELLO TEST, value = %d, value2 = %d\n", token.value, value2);
        return 0;
	*/

	u_int32_t *ptr = tasks[num1]->kernel_pre_v + num2;
	printk("value  %d\n", *ptr);
	return 0;
}

SYSCALL_DEFINE0(waittest)
{
	sema_init(&my_first_kernel_sem, 0);

	printk("WAITING ON SEMAPHORE\n");

	down(&my_first_kernel_sem);

	printk("SEMAPHORE DOWN ACTION SUCCESFULL\n");

	return 0;
}

SYSCALL_DEFINE0(releasetest)
{
	up(&my_first_kernel_sem);

	printk("SEMAPHORE UP ACTION SUCCESFULL\n");
	return 0;
}

SYSCALL_DEFINE1(releasejob, int,  task_id)
{
	//Allocate memory and initialize a new job structure
	struct job *new_job = kmalloc(sizeof(struct job), GFP_KERNEL);
	new_job->job_id = tasks[task_id]->last_job->job_id + 1;
	new_job->nodes_in_execution = 0;
    new_job->nodes_finished = 0;
	new_job->sec_guards = kcalloc((tasks[task_id]->number_of_sequences-1), sizeof(struct semaphore), GFP_KERNEL);
	new_job->previous_job = tasks[task_id]->last_job;

	//Initialize sec guards
    for(int i = 0; i < (tasks[task_id]->number_of_sequences -1); i++)
    {
        sema_init((new_job->sec_guards + i), 0);
    }

	//Initialize job lock
	mutex_init(new_job->job_lock);	


	tasks[task_id]->last_job->next_job = new_job;
	tasks[task_id]->last_job = new_job;


    //post primary guards
    for(int i = 0; i < tasks[task_id]->number_of_sequences; i++)
    {
        up(tasks[task_id]->prim_guards + i);
    }


	//printk("JOB RELEASED SUCCESFULLY  %d\n", task_id);
	return 0;
}

SYSCALL_DEFINE6(initializetask, int,  task_id, int, number_of_nodes, int, number_of_sequences, u_int32_t __user *, pre_v, u_int32_t __user *, pre_h, u_int32_t __user *, sequence_heads)
{

	//task id must be 1 or higher
	if(task_id < 1)
	{
		return 100;
	}

	printk("TASK CHECK SUCCESFULL \n");


	int ret = 0;

	if(tasks[task_id] != NULL)
	{
		kfree(tasks[task_id]);
	}

	printk("PTR CLEAR SUCCESFULL \n");

	int nr_bytes = number_of_nodes * 4;

	//Allocate memory for task data
	tasks[task_id] = kmalloc(sizeof(struct task_data), GFP_KERNEL);

	tasks[task_id]->number_of_nodes = number_of_nodes;
	tasks[task_id]->number_of_sequences = number_of_sequences;

	printk("TASK ALLOCATION SUCCESFULL \n");

	//Allocate memory for precdence bitmaps
	tasks[task_id]->kernel_pre_v = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);
	tasks[task_id]->kernel_pre_h = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);

	printk("TASK PRE ALLOCATION SUCCESFULL \n");

	//Allocate space for primary guards
	tasks[task_id]->prim_guards = kcalloc(number_of_sequences, sizeof(struct semaphore), GFP_KERNEL);

	printk("TASK GUARD ALLOCATION SUCCESFULL \n");

	//Copy data from user space
	ret = copy_from_user(tasks[task_id]->kernel_pre_v, pre_v, nr_bytes);
	ret = copy_from_user(tasks[task_id]->kernel_pre_v, pre_h, nr_bytes);
	ret = copy_from_user(tasks[task_id]->kernel_sequence_heads, sequence_heads, number_of_sequences * 4);

	printk("TASK CPY DATA FROM USER SPACE SUCCESFULL \n");


	//Initialize semaphores of the primary guards
	for(int i = 0; i < number_of_sequences; i++)
	{
		struct semaphore *sem_ptr = tasks[task_id]->prim_guards + i;
		sema_init(sem_ptr, 0);
	}

	printk("TASK GUARD INIT SUCCESFULL \n");
	
	//Allocate memory for the start of the linked list for jobs and initialize it
	tasks[task_id]->last_job = kmalloc(sizeof(struct job), GFP_KERNEL);
	tasks[task_id]->last_job->previous_job = NULL;
	tasks[task_id]->last_job->next_job = NULL;
	tasks[task_id]->last_job->sec_guards = NULL;
	tasks[task_id]->last_job->job_id = 0;

	printk("TASK LINKED LIST INIT SUCCESFULL \n");

	//Assign every sequence the start job
	for(int i = 0; i < number_of_sequences; i++)
	{
		tasks[task_id]->current_sequence_jobs[i] = tasks[task_id]->last_job;
	}


	printk("TASK INITIALIZED SUCCESFULLY \n");
	return 0;
}

SYSCALL_DEFINE2(waitjob, int,  task_id, int, sequence_id)
{

	//prim guard
	struct semaphore *guard_ptr = tasks[task_id]->prim_guards + (sequence_id - 1);
	down(guard_ptr);

	//Update job ponter
	tasks[task_id]->current_sequence_jobs[sequence_id-1] = tasks[task_id]->current_sequence_jobs[sequence_id-1]->next_job;

	//sec guard
	if(sequence_id != 1)
	{
		guard_ptr = tasks[task_id]->current_sequence_jobs[sequence_id-1]->sec_guards + (sequence_id-1);
		down(guard_ptr);
	}


	return 0;
}


SYSCALL_DEFINE3(tryexecute, int,  task_id, int, sequence_id, int, node_id)
{
	struct mutex *lock_ptr = tasks[task_id]->current_sequence_jobs[sequence_id-1]->job_lock;

	mutex_lock(lock_ptr);

    //Check if the next node can be executed if not unlock job_token and return
    if((rbs_check_precedence_constraints(task_id, sequence_id, node_id)) == false)
    {
        mutex_unlock(lock_ptr);
        return 1;
    }
    
    //Check if node is not already being executed if not unlock job_token and return
    if((rbs_check_if_node_in_execution(task_id, sequence_id, node_id)) == true)
    {
        mutex_unlock(lock_ptr);
        return 2;
    } 

	rbs_mark_node_in_execution(task_id, sequence_id, node_id);

	mutex_unlock(lock_ptr);

	return 0;
}


SYSCALL_DEFINE3(nodeexecuted, int,  task_id, int, sequence_id, int, node_id)
{
	struct mutex *lock_ptr = tasks[task_id]->current_sequence_jobs[sequence_id-1]->job_lock;
	mutex_lock(lock_ptr);

	rbs_mark_node_executed(task_id, sequence_id, node_id);

	mutex_unlock(lock_ptr);

	rbs_finish_job(task_id, sequence_id, node_id);

	return 0;
}


SYSCALL_DEFINE0(initializerbs)
{
	for(int i = 0; i < 100; i++)
	{
		tasks[i] = NULL;
	}

	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// END OF AREA WITH NEW RBS SYSTEM CALLS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
