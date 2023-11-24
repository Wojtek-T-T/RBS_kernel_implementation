//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// NEW SYSTEM CALLS ADDED AS A PART OF RBS PROJECT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
struct task_data *tasks[100];

bool rbs_check_precedence_constraints(int task_id, int sequence_id, int node_id)
{
	//source nodes doesn't have any precedence constraints
    if(node_id == 1)
    {
        return true;
    }


	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *current_sequence_job_ptr = seq_data->current_sequence_job;

	if(current_sequence_job_ptr == NULL)
	{
		printk("CPC ERROR CURRENT SEQ JOB PTR NULL");
	}

    
    u_int32_t *ptr = tasks[task_id]->kernel_pre_h + (node_id -1);
    if(*ptr == (*ptr & current_sequence_job_ptr->nodes_finished))
    {
		//printk("CURRENT EXECUTION STATE IS %d and PRE is %d, decision is true\n", current_sequence_job_ptr->nodes_finished, *ptr);
        return true;
    }
    else
    {
		//printk("CURRENT EXECUTION STATE IS %d and PRE is %d, decision is false\n", current_sequence_job_ptr->nodes_finished, *ptr);
        return false;
    } 

}

bool rbs_check_if_node_in_execution(int task_id, int sequence_id, int node_id)
{
    u_int32_t mask = 1;
    mask = mask << (node_id - 1);
	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *current_sequence_job_ptr = seq_data->current_sequence_job;
    u_int32_t local_execution_state = current_sequence_job_ptr->nodes_in_execution;

	if(current_sequence_job_ptr == NULL)
	{
		printk("CNE ERROR CURRENT SEQ JOB PTR NULL");
	}
    
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
	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *current_sequence_job_ptr = seq_data->current_sequence_job;

	if(current_sequence_job_ptr == NULL)
	{
		printk("MNIE ERROR CURRENT SEQ JOB PTR NULL");
	}

	current_sequence_job_ptr->nodes_in_execution = current_sequence_job_ptr->nodes_in_execution | mask;
}


void rbs_mark_node_executed(int task_id, int sequence_id, int node_id)
{
    //Mark task as finished by setting the bit in the job state variable
    u_int32_t mask = 1;
    mask = mask << (node_id - 1);
	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *current_sequence_job_ptr = seq_data->current_sequence_job;


	if(current_sequence_job_ptr == NULL)
	{
		printk("MNE ERROR CURRENT SEQ JOB PTR NULL");
	}


    current_sequence_job_ptr->nodes_finished = current_sequence_job_ptr->nodes_finished | mask;
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
            if(sequence > 1)
            {
                if(rbs_check_precedence_constraints(task_id, sequence_id, i+1))
                {
                    //SIGNAL
					struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
					struct job *current_sequence_job_ptr = seq_data->current_sequence_job;

					if(current_sequence_job_ptr == NULL)
					{
						printk("SIG ERROR CURRENT SEQ JOB PTR NULL");
					}

					


					struct semaphore *guard_to_signal = current_sequence_job_ptr->sec_guards + sequence -2;

					/*
					if(guard_to_signal == NULL)
					{
						printk("SIG ERROR SEM PTR NULL");
					}
					*/
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
	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *old_job = seq_data->current_sequence_job;
    if(old_job->previous_job == NULL)
    {
        return;
    }

    //Destroy mutex
    kfree(old_job->previous_job->job_lock);


	//Destroy semaphores
	kfree(old_job->previous_job->sec_guards);

	//Destroy job
	kfree(old_job->previous_job);

    	
	return;
}


SYSCALL_DEFINE1(RBSrelease_job, int,  task_id)
{
	//Allocate memory and initialize a new job structure
	struct job *new_job = kmalloc(sizeof(struct job), GFP_KERNEL);
	new_job->job_id = tasks[task_id]->last_job->job_id + 1;
	new_job->nodes_in_execution = 0;
    new_job->nodes_finished = 0;
	new_job->sec_guards = kcalloc((tasks[task_id]->number_of_sequences-1), sizeof(struct semaphore), GFP_KERNEL);
	new_job->previous_job = tasks[task_id]->last_job;
	new_job->job_lock = kmalloc(sizeof(struct mutex), GFP_KERNEL);

	//Initialize sec guards
    for(int i = 0; i < (tasks[task_id]->number_of_sequences -1); i++)
    {
		struct semaphore *sem_ptr = new_job->sec_guards + i;
		if(sem_ptr == NULL)
		{
			return 21;
		}
        sema_init(sem_ptr, 0);
    }

	//Initialize job lock
	if(new_job->job_lock == NULL)
	{
		return 22;
	}
	mutex_init(new_job->job_lock);	

	//printk("JOB RELEASE MUTEX INITIALIZE SUCCESFULLY  %d\n", task_id);


	tasks[task_id]->last_job->next_job = new_job;
	


    //post primary guards
    for(int i = 0; i < tasks[task_id]->number_of_sequences; i++)
    {
		struct semaphore *sem_ptr = tasks[task_id]->prim_guards + i;
		if(sem_ptr == NULL)
		{
		return 23;
		}

        up(sem_ptr);
    }

	tasks[task_id]->last_job = new_job;


	//printk("JOB RELEASED SUCCESFULLY  task %d, job %d\n", task_id, new_job->job_id);
	return 0;
}

SYSCALL_DEFINE4(RBSdata_transfer, int,  task_id, u_int32_t __user *, pre_v, u_int32_t __user *, pre_h, u_int32_t __user *, sequence_heads)
{
	if(tasks[task_id] == NULL)
	{
		return 24;
	}
	int ret = 0;
	int nr_bytes = tasks[task_id]->number_of_nodes * 4;

	//Copy data from user space
	ret = copy_from_user(tasks[task_id]->kernel_pre_v, pre_v, nr_bytes);
	//printk("TASK CPY DATA FROM USER SPACE SUCCESFULL (pre v) %d\n", ret);
	ret = copy_from_user(tasks[task_id]->kernel_pre_h, pre_h, nr_bytes);
	//printk("TASK CPY DATA FROM USER SPACE SUCCESFULL (pre h) %d\n", ret);
	ret = copy_from_user(tasks[task_id]->kernel_sequence_heads, sequence_heads, nr_bytes);
	//printk("TASK CPY DATA FROM USER SPACE SUCCESFULL (cesss heads) %d\n", ret);

	/*
	for(int i = 0; i < tasks[task_id]->number_of_nodes; i++)
	{
		u_int32_t * v_ptr =   tasks[task_id]->kernel_pre_v + i;
		u_int32_t * h_ptr =   tasks[task_id]->kernel_pre_h + i;

		printk("v = %d, h = %d \n", *v_ptr, *h_ptr);
	}
	*/



	return 0;

}

SYSCALL_DEFINE3(RBSinitialize_task, int,  task_id, int, number_of_nodes, int, number_of_sequences)
{

	//task id must be 1 or higher
	if(task_id < 1)
	{
		return 25;
	}

	//printk("TASK CHECK SUCCESFULL \n");



	//Allocate memory for task data
	tasks[task_id] = kmalloc(sizeof(struct task_data), GFP_KERNEL);

	tasks[task_id]->number_of_nodes = number_of_nodes;
	tasks[task_id]->number_of_sequences = number_of_sequences;

	//printk("TASK ALLOCATION SUCCESFULL \n");

	//Allocate memory for precdence bitmaps
	tasks[task_id]->kernel_pre_v = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);
	tasks[task_id]->kernel_pre_h = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);
	tasks[task_id]->kernel_sequence_heads = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);

	//printk("TASK PRE ALLOCATION SUCCESFULL \n");

	//Allocate space for primary guards
	tasks[task_id]->prim_guards = kcalloc(number_of_sequences, sizeof(struct semaphore), GFP_KERNEL);

	//printk("TASK GUARD ALLOCATION SUCCESFULL \n");


	//Initialize semaphores of the primary guards
	for(int i = 0; i < number_of_sequences; i++)
	{
		struct semaphore *sem_ptr = tasks[task_id]->prim_guards + i;
		sema_init(sem_ptr, 0);
	}

	//printk("TASK GUARD INIT SUCCESFULL \n");
	
	//Allocate memory for the start of the linked list for jobs and initialize it
	tasks[task_id]->last_job = kmalloc(sizeof(struct job), GFP_KERNEL);
	tasks[task_id]->last_job->job_lock = NULL;
	tasks[task_id]->last_job->previous_job = NULL;
	tasks[task_id]->last_job->next_job = NULL;
	tasks[task_id]->last_job->sec_guards = NULL;
	tasks[task_id]->last_job->job_id = 0;

	//printk("TASK LINKED LIST INIT SUCCESFULL \n");

	//Assign every sequence the start job
	tasks[task_id]->sequences_data = kcalloc(number_of_sequences, sizeof(struct sequence_data), GFP_KERNEL);
	for(int i = 0; i < number_of_sequences; i++)
	{
		struct sequence_data *seq_data_ptr = tasks[task_id]->sequences_data + i;
		seq_data_ptr->current_sequence_job = tasks[task_id]->last_job;
	}


	printk("TASK INITIALIZED SUCCESFULLY task %d\n", task_id);
	return 0;
}

SYSCALL_DEFINE2(RBSwait_job, int,  task_id, int, sequence_id)
{

	//prim guard
	//printk("WAITING ON PRIM GUARD task %d, seq: %d, node %d\n", task_id, sequence_id, node_id);
	struct semaphore *guard_ptr = tasks[task_id]->prim_guards + (sequence_id - 1);
	if(guard_ptr == NULL)
	{
		return 26;
	}
	down(guard_ptr);

	//Update job ponter
	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *new_job_ptr = seq_data->current_sequence_job->next_job;
	

	if(new_job_ptr == NULL)
	{
		return 27;
	}
	seq_data->current_sequence_job = new_job_ptr;

	//sec guard
	if(sequence_id > 1)
	{
		//printk("WAITING ON SEC GUARD task %d, seq: %d, node %d\n", task_id, sequence_id, node_id);
		struct semaphore *sec_guard_ptr = seq_data->current_sequence_job->sec_guards + (sequence_id-2);

		if(sec_guard_ptr == NULL)
		{
			return 28;
		}

		down(sec_guard_ptr);
	}
	else
	{
		;
	}

	printk("WAIT WAS SUCCESFUL task %d, sequence %d ,job: %d\n", task_id, sequence_id, new_job_ptr->job_id);
	return 0;
}


SYSCALL_DEFINE3(RBStry_execute, int,  task_id, int, sequence_id, int, node_id)
{
	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *current_sequence_job_ptr = seq_data->current_sequence_job;
	struct mutex *lock_ptr = current_sequence_job_ptr->job_lock;

	if(current_sequence_job_ptr == NULL)
	{
		return 29;
	}
	if(current_sequence_job_ptr->previous_job == NULL)
	{
		return 51;
	}
	if(lock_ptr == NULL)
	{
		return 30;
	}

	mutex_lock(lock_ptr);

    //Check if the next node can be executed if not unlock job_token and return
    if((rbs_check_precedence_constraints(task_id, sequence_id, node_id)) == false)
    {
        mutex_unlock(lock_ptr);
		//printk("EXECUTION WAS NOT SUCCESFUL (pre cons) task %d, seq: %d, node %d\n", task_id, sequence_id, node_id);
        return 1;
    }
    
    //Check if node is not already being executed if not unlock job_token and return
    if((rbs_check_if_node_in_execution(task_id, sequence_id, node_id)) == true)
    {
        mutex_unlock(lock_ptr);
		//printk("EXECUTION WAS NOT SUCCESFUL (node in ex) task %d, seq: %d, node %d\n", task_id, sequence_id, node_id);
        return 2;
    } 

	rbs_mark_node_in_execution(task_id, sequence_id, node_id);

	mutex_unlock(lock_ptr);

	//printk("EXECUTION WAS SUCCESFUL task %d, seq: %d, node %d, job %d\n", task_id, sequence_id, node_id, current_sequence_job_ptr->job_id);

	return 0;
}


SYSCALL_DEFINE3(RBSnode_executed, int,  task_id, int, sequence_id, int, node_id)
{
	struct sequence_data *seq_data = tasks[task_id]->sequences_data + (sequence_id - 1);
	struct job *current_sequence_job_ptr = seq_data->current_sequence_job;
	struct mutex *lock_ptr = current_sequence_job_ptr->job_lock;

	if(current_sequence_job_ptr == NULL)
	{
		return 31;
	}
	if(lock_ptr == NULL)
	{
		return 32;
	}

	mutex_lock(lock_ptr);

	rbs_mark_node_executed(task_id, sequence_id, node_id);

	mutex_unlock(lock_ptr);

	rbs_signal_sequence(task_id, sequence_id, node_id);

	if(node_id == tasks[task_id]->number_of_nodes)
	{
		rbs_finish_job(task_id, sequence_id, node_id);
	}
	

	//printk("EXECUTION MARKING  WAS SUCCESFUL seq: %d, node %d\n", sequence_id, node_id);

	return 0;
}


SYSCALL_DEFINE0(RBSinitialize_rbs)
{
	for(int i = 0; i < 100; i++)
	{
		tasks[i] = NULL;
	}

	return 0;
}

SYSCALL_DEFINE1(RBSdelete_task, int, task_id)
{

	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// END OF AREA WITH NEW RBS SYSTEM CALLS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
