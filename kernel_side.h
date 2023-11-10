//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// NEW SYSTEM CALLS ADDED AS A PART OF RBS PROJECT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

struct job_token
{
	int value;
};

struct job
{
	struct job *next_job;
	struct job *previous_job;

	int job_id;
	u_int32_t nodes_in_execution;
	u_int32_t nodes_finished;

	struct semaphore *sec_guards;
	struct mutex *job_lock;
};


struct task_data
{
	int task_id;
	int number_of_sequences;
	int number_of_nodes;
	struct semaphore *prim_guards;
	u_int32_t *kernel_pre_v;
	u_int32_t *kernel_pre_h;
	struct job *last_job;

};


////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// END OF AREA WITH NEW RBS SYSTEM CALLS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////