//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// NEW SYSTEM CALLS ADDED AS A PART OF RBS PROJECT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
int value2 = 67;
struct semaphore my_first_kernel_sem;

struct task_data *tasks[100];

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

SYSCALL_DEFINE5(initializetask, int,  task_id, int, number_of_nodes, int, number_of_sequences, u_int32_t __user *, pre_v, u_int32_t __user *, pre_h)
{
	int ret = 0;

	if(tasks[task_id] != NULL)
	{
		kfree(tasks[task_id]);
	}

	int nr_bytes = number_of_nodes * 4;

	//Allocate memory for task data
	tasks[task_id] = kmalloc(sizeof(struct task_data), GFP_KERNEL);

	tasks[task_id]->number_of_nodes = number_of_nodes;
	tasks[task_id]->number_of_sequences = number_of_sequences;

	//Allocate memory for precdence bitmaps
	tasks[task_id]->kernel_pre_v = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);
	tasks[task_id]->kernel_pre_h = kcalloc(number_of_nodes, sizeof(u_int32_t), GFP_KERNEL);

	//Allocate space for primary guards
	tasks[task_id]->prim_guards = kcalloc(number_of_sequences, sizeof(struct semaphore), GFP_KERNEL);

	//Copy data from user space
	ret = copy_from_user(tasks[task_id]->kernel_pre_v, pre_v, nr_bytes);
	ret = copy_from_user(tasks[task_id]->kernel_pre_v, pre_h, nr_bytes);


	//Initialize semaphores of the primary guards
	for(int i = 0; i < number_of_sequences; i++)
	{
		struct semaphore *sem_ptr = tasks[task_id]->prim_guards + i;
		sema_init(sem_ptr, 0);
	}
	
	//Allocate memory for the start of the linked list for jobs and initialize it
	tasks[task_id]->last_job = kmalloc(sizeof(struct job), GFP_KERNEL);
	tasks[task_id]->last_job->previous_job = NULL;
	tasks[task_id]->last_job->next_job = NULL;
	tasks[task_id]->last_job->job_id = 0;

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


SYSCALL_DEFINE2(tryexecute, int,  task_id, int, sequence_id, int, node_id)
{
	
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// END OF AREA WITH NEW RBS SYSTEM CALLS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
