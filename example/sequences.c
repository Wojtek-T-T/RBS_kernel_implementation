#include "sequences.h"
//semaphores
sem_t semaphores_T1[23];
sem_t semaphores_T2[6];
sem_t semaphores_T3[2];
sem_t semaphores_T4[9];
//threads
pthread_t task1_threads[23];
pthread_t task2_threads[6];
pthread_t task3_threads[2];
pthread_t task4_threads[9];

//sequence functions pointers
void *(*seq_func_ptr_t1[23])() = {sequence_1_1_function, sequence_1_2_function, sequence_1_3_function, sequence_1_4_function, sequence_1_5_function, sequence_1_6_function, sequence_1_7_function, sequence_1_8_function, sequence_1_9_function, sequence_1_10_function, sequence_1_11_function, sequence_1_12_function, sequence_1_13_function, sequence_1_14_function, sequence_1_15_function, sequence_1_16_function, sequence_1_17_function, sequence_1_18_function, sequence_1_19_function, sequence_1_20_function, sequence_1_21_function, sequence_1_22_function, sequence_1_23_function};
void *(*seq_func_ptr_t2[6])() = {sequence_2_1_function, sequence_2_2_function, sequence_2_3_function, sequence_2_4_function, sequence_2_5_function, sequence_2_6_function};
void *(*seq_func_ptr_t3[2])() = {sequence_3_1_function, sequence_3_2_function};
void *(*seq_func_ptr_t4[9])() = {sequence_4_1_function, sequence_4_2_function, sequence_4_3_function, sequence_4_4_function, sequence_4_5_function, sequence_4_6_function, sequence_4_7_function, sequence_4_8_function, sequence_4_9_function};

//node functions pointers
void (*nodes_func_ptr_t1[35])() = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7, node_1_8, node_1_9, node_1_10, node_1_11, node_1_12, node_1_13, node_1_14, node_1_15, node_1_16, node_1_17, node_1_18, node_1_19, node_1_20, node_1_21, node_1_22, node_1_23, node_1_24, node_1_25, node_1_26, node_1_27, node_1_28, node_1_29, node_1_30, node_1_31, node_1_32, node_1_33, node_1_34, node_1_35};
void (*nodes_func_ptr_t2[12])() = {node_2_1, node_2_2, node_2_3, node_2_4, node_2_5, node_2_6, node_2_7, node_2_8, node_2_9, node_2_10, node_2_11, node_2_12};
void (*nodes_func_ptr_t3[4])() = {node_3_1, node_3_2, node_3_3, node_3_4};
void (*nodes_func_ptr_t4[15])() = {node_4_1, node_4_2, node_4_3, node_4_4, node_4_5, node_4_6, node_4_7, node_4_8, node_4_9, node_4_10, node_4_11, node_4_12, node_4_13, node_4_14, node_4_15};

void *(* seq_func_ptr[40])() = {sequence_1_1_function, sequence_1_2_function, sequence_1_3_function, sequence_1_4_function, sequence_1_5_function, sequence_1_6_function, sequence_1_7_function, sequence_1_8_function, sequence_1_9_function, sequence_1_10_function, sequence_1_11_function, sequence_1_12_function, sequence_1_13_function, sequence_1_14_function, sequence_1_15_function, sequence_1_16_function, sequence_1_17_function, sequence_1_18_function, sequence_1_19_function, sequence_1_20_function, sequence_1_21_function, sequence_1_22_function, sequence_1_23_function, sequence_2_1_function, sequence_2_2_function, sequence_2_3_function, sequence_2_4_function, sequence_2_5_function, sequence_2_6_function, sequence_3_1_function, sequence_3_2_function, sequence_4_1_function, sequence_4_2_function, sequence_4_3_function, sequence_4_4_function, sequence_4_5_function, sequence_4_6_function, sequence_4_7_function, sequence_4_8_function, sequence_4_9_function};

//precedence constraints
uint64_t T1_precedence_constraints_h[35] = {0, 1, 2, 2, 2, 2, 2, 2, 252, 1, 512, 512, 512, 512, 15360, 1, 32768, 32768, 32768, 32768, 983040, 1, 2097152, 2097152, 2097152, 2097152, 2097152, 2097152, 264241152, 1, 536870912, 536870912, 536870912, 7516192768, 8859435264};
uint64_t T1_precedence_constraints_v[35] = {539001346, 252, 256, 256, 256, 256, 256, 256, 17179869184, 15360, 16384, 16384, 16384, 16384, 17179869184, 983040, 1048576, 1048576, 1048576, 1048576, 17179869184, 264241152, 268435456, 268435456, 268435456, 268435456, 268435456, 268435456, 17179869184, 7516192768, 8589934592, 8589934592, 8589934592, 17179869184, 0};
uint64_t T2_precedence_constraints_h[12] = {0, 1, 2, 2, 2, 28, 1, 64, 64, 64, 896, 1056};
uint64_t T2_precedence_constraints_v[12] = {66, 28, 32, 32, 32, 2048, 896, 1024, 1024, 1024, 2048, 0};
uint64_t T3_precedence_constraints_h[4] = {0, 1, 1, 6};
uint64_t T3_precedence_constraints_v[4] = {6, 8, 8, 0};
uint64_t T4_precedence_constraints_h[15] = {0, 1, 2, 2, 2, 2, 2, 2, 252, 1, 512, 512, 512, 7168, 8448};
uint64_t T4_precedence_constraints_v[15] = {514, 252, 256, 256, 256, 256, 256, 256, 16384, 7168, 8192, 8192, 8192, 16384, 0};

//sequences heads
uint64_t T1_sequence_heads[35] = {1, 0, 0, 23, 22, 21, 20, 19, 0, 15, 0, 18, 17, 16, 0, 11, 0, 14, 13, 12, 0, 5, 0, 10, 9, 8, 7, 6, 0, 2, 0, 4, 3, 0, 0};
uint64_t T2_sequence_heads[12] = {1, 0, 0, 6, 5, 0, 2, 0, 4, 3, 0, 0};
uint64_t T3_sequence_heads[4] = {1, 0, 2, 0};
uint64_t T4_sequence_heads[15] = {1, 0, 0, 9, 8, 7, 6, 5, 0, 2, 0, 4, 3, 0, 0};

//task data structures
struct task_data task1_data = { .task_id =1, .priority =98, .number_of_nodes =35, .number_of_sequences = 23, 
.job_counter = 0, .pre_cons_h = T1_precedence_constraints_h, .pre_cons_v = T1_precedence_constraints_v, 
.sequence_heads = T1_sequence_heads, .sequences_guards = semaphores_T1, .last_added_job = NULL, .seq_threads = task1_threads, .period = 13818, 
.func = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7, node_1_8, node_1_9, node_1_10, node_1_11, node_1_12, node_1_13, node_1_14, node_1_15, node_1_16, node_1_17, node_1_18, node_1_19, node_1_20, node_1_21, node_1_22, node_1_23, node_1_24, node_1_25, node_1_26, node_1_27, node_1_28, node_1_29, node_1_30, node_1_31, node_1_32, node_1_33, node_1_34, node_1_35}}; 
struct task_data task2_data = { .task_id =2, .priority =96, .number_of_nodes =12, .number_of_sequences = 6, 
.job_counter = 0, .pre_cons_h = T2_precedence_constraints_h, .pre_cons_v = T2_precedence_constraints_v, 
.sequence_heads = T2_sequence_heads, .sequences_guards = semaphores_T2, .last_added_job = NULL, .seq_threads = task2_threads, .period = 20874, 
.func = {node_2_1, node_2_2, node_2_3, node_2_4, node_2_5, node_2_6, node_2_7, node_2_8, node_2_9, node_2_10, node_2_11, node_2_12}}; 
struct task_data task3_data = { .task_id =3, .priority =95, .number_of_nodes =4, .number_of_sequences = 2, 
.job_counter = 0, .pre_cons_h = T3_precedence_constraints_h, .pre_cons_v = T3_precedence_constraints_v, 
.sequence_heads = T3_sequence_heads, .sequences_guards = semaphores_T3, .last_added_job = NULL, .seq_threads = task3_threads, .period = 28308, 
.func = {node_3_1, node_3_2, node_3_3, node_3_4}}; 
struct task_data task4_data = { .task_id =4, .priority =97, .number_of_nodes =15, .number_of_sequences = 9, 
.job_counter = 0, .pre_cons_h = T4_precedence_constraints_h, .pre_cons_v = T4_precedence_constraints_v, 
.sequence_heads = T4_sequence_heads, .sequences_guards = semaphores_T4, .last_added_job = NULL, .seq_threads = task4_threads, .period = 14532, 
.func = {node_4_1, node_4_2, node_4_3, node_4_4, node_4_5, node_4_6, node_4_7, node_4_8, node_4_9, node_4_10, node_4_11, node_4_12, node_4_13, node_4_14, node_4_15}}; 

struct task_data *tasks_data[20] = {&task1_data, &task2_data, &task3_data, &task4_data};

//sequences functions
void *sequence_1_1_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_2_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,30) != 0)
   {
     TerminateSequence(seq_data, 30);
     continue;
   }

   if(RBS_Execute(seq_data,31) != 0)
   {
     TerminateSequence(seq_data, 31);
     continue;
   }

   if(RBS_Execute(seq_data,34) != 0)
   {
     TerminateSequence(seq_data, 34);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_3_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,33) != 0)
   {
     TerminateSequence(seq_data, 33);
     continue;
   }

   if(RBS_Execute(seq_data,34) != 0)
   {
     TerminateSequence(seq_data, 34);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_4_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,32) != 0)
   {
     TerminateSequence(seq_data, 32);
     continue;
   }

   if(RBS_Execute(seq_data,34) != 0)
   {
     TerminateSequence(seq_data, 34);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_5_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
    RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,22) != 0)
   {
     TerminateSequence(seq_data, 22);
     continue;
   }

   if(RBS_Execute(seq_data,23) != 0)
   {
     TerminateSequence(seq_data, 23);
     continue;
   }

   if(RBS_Execute(seq_data,29) != 0)
   {
     TerminateSequence(seq_data, 29);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_6_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,28) != 0)
   {
     TerminateSequence(seq_data, 28);
     continue;
   }

   if(RBS_Execute(seq_data,29) != 0)
   {
     TerminateSequence(seq_data, 29);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_7_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,27) != 0)
   {
     TerminateSequence(seq_data, 27);
     continue;
   }

   if(RBS_Execute(seq_data,29) != 0)
   {
     TerminateSequence(seq_data, 29);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_8_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
  RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,26) != 0)
   {
     TerminateSequence(seq_data, 26);
     continue;
   }

   if(RBS_Execute(seq_data,29) != 0)
   {
     TerminateSequence(seq_data, 29);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_9_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,25) != 0)
   {
     TerminateSequence(seq_data, 25);
     continue;
   }

   if(RBS_Execute(seq_data,29) != 0)
   {
     TerminateSequence(seq_data, 29);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_10_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,24) != 0)
   {
     TerminateSequence(seq_data, 24);
     continue;
   }

   if(RBS_Execute(seq_data,29) != 0)
   {
     TerminateSequence(seq_data, 29);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_11_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,16) != 0)
   {
     TerminateSequence(seq_data, 16);
     continue;
   }

   if(RBS_Execute(seq_data,17) != 0)
   {
     TerminateSequence(seq_data, 17);
     continue;
   }

   if(RBS_Execute(seq_data,21) != 0)
   {
     TerminateSequence(seq_data, 21);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_12_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,20) != 0)
   {
     TerminateSequence(seq_data, 20);
     continue;
   }

   if(RBS_Execute(seq_data,21) != 0)
   {
     TerminateSequence(seq_data, 21);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_13_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,19) != 0)
   {
     TerminateSequence(seq_data, 19);
     continue;
   }

   if(RBS_Execute(seq_data,21) != 0)
   {
     TerminateSequence(seq_data, 21);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_14_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,18) != 0)
   {
     TerminateSequence(seq_data, 18);
     continue;
   }

   if(RBS_Execute(seq_data,21) != 0)
   {
     TerminateSequence(seq_data, 21);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_15_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_16_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,14) != 0)
   {
     TerminateSequence(seq_data, 14);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_17_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);


   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_18_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_19_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_20_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_21_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_22_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_23_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,35) != 0)
   {
     TerminateSequence(seq_data, 35);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_1_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_2_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_3_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_4_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_5_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_6_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_1_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_2_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_1_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_2_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,14) != 0)
   {
     TerminateSequence(seq_data, 14);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_3_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   if(RBS_Execute(seq_data,14) != 0)
   {
     TerminateSequence(seq_data, 14);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_4_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   if(RBS_Execute(seq_data,14) != 0)
   {
     TerminateSequence(seq_data, 14);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_5_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_6_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_7_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_8_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_9_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   int stop = RBS_Wait(seq_data);
   if(stop == 1)
   {
      return 0;
   }

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,15) != 0)
   {
     TerminateSequence(seq_data, 15);
     continue;
   }

   FinishJob(seq_data);
  }
}
