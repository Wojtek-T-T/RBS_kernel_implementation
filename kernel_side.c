//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// NEW SYSTEM CALLS ADDED AS A PART OF RBS PROJECT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
struct task_data *tasks[100];

bool rbs_check_precedence_constraints(int task_id, int sequence_id, int node_id)
{
	struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
    //source nodes doesn't have any precedence constraints
    if(node_id == 1)
    {
        return true;
    }
    
    u_int32_t *ptr = tasks[task_id]->kernel_pre_h + (node_id -1);
    if(*ptr == (*ptr & current_sequence_job_ptr->nodes_finished))
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
	struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
    u_int32_t local_execution_state = current_sequence_job_ptr->nodes_in_execution;
    
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
	struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
	current_sequence_job_ptr->nodes_in_execution = current_sequence_job_ptr->nodes_in_execution | mask;
}


void rbs_mark_node_executed(int task_id, int sequence_id, int node_id)
{
    //Mark task as finished by setting the bit in the job state variable
    u_int32_t mask = 1;
    mask = mask << (node_id - 1);
	struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
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
            if(sequence != 0)
            {
                if(rbs_check_precedence_constraints(task_id, sequence_id, i+1))
                {
                    //SIGNAL
					struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
					struct semaphore *guard_to_signal = current_sequence_job_ptr->sec_guards + sequence -2;
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
   // if(tasks[task_id]->current_sequence_jobs[sequence_id - 1]->sec_guards == NULL)
   // {
      //  return;
   // }

    //Destroy mutex
   // pthread_mutex_destroy(&sequenceDATA->current_job->previous_job->job_lock); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //Destroy secondary semaphores
    //for(int i = 0; i < tasks[task_id]->number_of_sequences; i++)
    //{
        //sem_destroy((sequenceDATA->current_job->previous_job->secondary_sequences_guards + i));
    //}
    
    //Free memory
   // kfree(tasks[task_id]->current_sequence_jobs[sequence_id - 1]->previous_job->sec_guards); ///!!!!!
    //kfree(tasks[task_id]->current_sequence_jobs[sequence_id - 1]->previous_job);	
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
			return 99;
		}
        sema_init(sem_ptr, 0);
    }

	//Initialize job lock
	if(new_job->job_lock == NULL)
	{
		return 99;
	}
	mutex_init(new_job->job_lock);	

	printk("JOB RELEASE MUTEX INITIALIZE SUCCESFULLY  %d\n", task_id);


	tasks[task_id]->last_job->next_job = new_job;
	


    //post primary guards
    for(int i = 0; i < tasks[task_id]->number_of_sequences; i++)
    {
		struct semaphore *sem_ptr = tasks[task_id]->prim_guards + i;
		if(sem_ptr == NULL)
		{
		return 99;
		}

        up(sem_ptr);
    }

	tasks[task_id]->last_job = new_job;


	printk("JOB RELEASED SUCCESFULLY  %d\n", task_id);
	return 0;
}

SYSCALL_DEFINE4(RBScopy_data_from_user, int,  task_id, u_int32_t __user *, pre_v, u_int32_t __user *, pre_h, u_int32_t __user *, sequence_heads)
{
	if(tasks[task_id] == NULL)
	{
		return 99;
	}
	int ret = 0;
	int nr_bytes = tasks[task_id]->number_of_nodes * 4;

	//Copy data from user space
	ret = copy_from_user(tasks[task_id]->kernel_pre_v, pre_v, nr_bytes);
	ret = copy_from_user(tasks[task_id]->kernel_pre_v, pre_h, nr_bytes);
	ret = copy_from_user(tasks[task_id]->kernel_sequence_heads, sequence_heads, nr_bytes);

	printk("TASK CPY DATA FROM USER SPACE SUCCESFULL \n");

	return 0;

}

SYSCALL_DEFINE3(RBSinitialize_task, int,  task_id, int, number_of_nodes, int, number_of_sequences)
{

	//task id must be 1 or higher
	if(task_id < 1)
	{
		return 100;
	}

	printk("TASK CHECK SUCCESFULL \n");


	int ret = 0;

	printk("PTR CLEAR SUCCESFULL \n");


	//Allocate memory for task data
	tasks[task_id] = kmalloc(sizeof(struct task_data), GFP_KERNEL);

	tasks[task_id]->number_of_nodes = number_of_nodes;
	tasks[task_id]->number_of_sequences = number_of_sequences;

	printk("TASK ALLOCATION SUCCESFULL \n");

	//Allocate memory for precdence bitmaps
	tasks[task_id]->kernel_pre_v = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);
	tasks[task_id]->kernel_pre_h = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);
	tasks[task_id]->kernel_sequence_heads = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);

	printk("TASK PRE ALLOCATION SUCCESFULL \n");

	//Allocate space for primary guards
	tasks[task_id]->prim_guards = kcalloc(number_of_sequences, sizeof(struct semaphore), GFP_KERNEL);

	printk("TASK GUARD ALLOCATION SUCCESFULL \n");


	//Initialize semaphores of the primary guards
	for(int i = 0; i < number_of_sequences; i++)
	{
		struct semaphore *sem_ptr = tasks[task_id]->prim_guards + i;
		sema_init(sem_ptr, 0);
	}

	printk("TASK GUARD INIT SUCCESFULL \n");
	
	//Allocate memory for the start of the linked list for jobs and initialize it
	tasks[task_id]->last_job = kmalloc(sizeof(struct job), GFP_KERNEL);
	tasks[task_id]->last_job->job_lock = NULL;
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

SYSCALL_DEFINE2(RBSwait_job, int,  task_id, int, sequence_id)
{

	//prim guard
	printk("WAITING ON PRIM GUARD %d\n", sequence_id);
	struct semaphore *guard_ptr = tasks[task_id]->prim_guards + (sequence_id - 1);
	if(guard_ptr == NULL)
	{
		return 99;
	}
	down(guard_ptr);

	//Update job ponter
	struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
	struct job *new_job_ptr = current_sequence_job_ptr->next_job;

	if(new_job_ptr == NULL)
	{
		return 99;
	}
	current_sequence_job_ptr = new_job_ptr;

	//sec guard
	if(sequence_id > 1)
	{
		printk("WAITING ON SEC GUARD %d\n", sequence_id);
		struct semaphore *sec_guard_ptr = current_sequence_job_ptr->sec_guards + (sequence_id-2);

		if(sec_guard_ptr == NULL)
		{
			return 99;
		}

		down(sec_guard_ptr);
	}
	else
	{
		;
	}

	printk("WAIT WAS SUCCESFUL %d\n", sequence_id);
	return 0;
}


SYSCALL_DEFINE3(RBStry_execute, int,  task_id, int, sequence_id, int, node_id)
{
	struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
	struct mutex *lock_ptr = current_sequence_job_ptr->job_lock;

	if(current_sequence_job_ptr == NULL)
	{
		return 99;
	}
	if(lock_ptr == NULL)
	{
		return 99;
	}

	mutex_lock(lock_ptr);

    //Check if the next node can be executed if not unlock job_token and return
    if((rbs_check_precedence_constraints(task_id, sequence_id, node_id)) == false)
    {
        mutex_unlock(lock_ptr);
		printk("EXECUTION WAS NOT SUCCESFUL (pre cons) seq: %d, node %d\n", sequence_id, node_id);
        return 1;
    }
    
    //Check if node is not already being executed if not unlock job_token and return
    if((rbs_check_if_node_in_execution(task_id, sequence_id, node_id)) == true)
    {
        mutex_unlock(lock_ptr);
		printk("EXECUTION WAS NOT SUCCESFUL (node in ex) seq: %d, node %d\n", sequence_id, node_id);
        return 2;
    } 

	rbs_mark_node_in_execution(task_id, sequence_id, node_id);

	mutex_unlock(lock_ptr);

	printk("EXECUTION WAS SUCCESFUL seq: %d, node %d\n", sequence_id, node_id);

	return 0;
}


SYSCALL_DEFINE3(RBSnode_executed, int,  task_id, int, sequence_id, int, node_id)
{
	struct job *current_sequence_job_ptr = tasks[task_id]->current_sequence_jobs[sequence_id - 1];
	struct mutex *lock_ptr = current_sequence_job_ptr->job_lock;

	if(current_sequence_job_ptr == NULL)
	{
		return 99;
	}
	if(lock_ptr == NULL)
	{
		return 99;
	}

	mutex_lock(lock_ptr);

	rbs_mark_node_executed(task_id, sequence_id, node_id);

	mutex_unlock(lock_ptr);

	rbs_signal_sequence(task_id, sequence_id, node_id);

	//rbs_finish_job(task_id, sequence_id, node_id);

	printk("EXECUTION MARKING  WAS SUCCESFUL seq: %d, node %d\n", sequence_id, node_id);

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
