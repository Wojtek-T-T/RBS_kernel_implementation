//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// NEW SYSTEM CALLS ADDED AS A PART OF RBS PROJECT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


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
	u_int32_t *kernel_sequence_heads;
	struct job *last_job;
	struct sequence_data *sequences_data;

};

struct sequence_data
{
	int sequence_id;
	struct job *current_sequence_job;

};

bool rbs_check_precedence_constraints(int task_id, int sequence_id, int node_id);
bool rbs_check_if_node_in_execution(int task_id, int sequence_id, int node_id);
void rbs_mark_node_in_execution(int task_id, int sequence_id, int node_id);
void rbs_mark_node_executed(int task_id, int sequence_id, int node_id);
void rbs_signal_sequence(int task_id, int sequence_id, int node_id);
void rbs_finish_job(int task_id, int sequence_id, int node_id);
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// END OF AREA WITH NEW RBS SYSTEM CALLS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
