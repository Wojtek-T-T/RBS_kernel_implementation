#ifndef SEQUENCES_H
#define SEQUENCES_H
#include <stdint.h>
#include "../rbs_lib_kernel/rbs_lib_kernel.h" 
#include "workload.h"
#define number_of_tasks 4

//tasks data structures
extern struct task_data task1_data;
extern struct task_data task2_data;
extern struct task_data task3_data;
extern struct task_data task4_data;
extern struct task_data *tasks_data[20];

//sequence functions pointers
extern void *(*seq_func_ptr_t1[23])();
extern void *(*seq_func_ptr_t2[6])();
extern void *(*seq_func_ptr_t3[2])();
extern void *(*seq_func_ptr_t4[9])();

 extern void *(*seq_func_ptr[40])();

//node functions pointers
extern void (*nodes_func_ptr_t1[35])();
extern void (*nodes_func_ptr_t2[12])();
extern void (*nodes_func_ptr_t3[4])();
extern void (*nodes_func_ptr_t4[15])();

//horizontal and vertical precedence constraints
extern uint64_t T1_precedence_constraints_h[35];
extern uint64_t T1_precedence_constraints_v[35];
extern uint64_t T2_precedence_constraints_h[12];
extern uint64_t T2_precedence_constraints_v[12];
extern uint64_t T3_precedence_constraints_h[4];
extern uint64_t T3_precedence_constraints_v[4];
extern uint64_t T4_precedence_constraints_h[15];
extern uint64_t T4_precedence_constraints_v[15];

//arrays with sequence heads
extern uint64_t T1_sequence_heads[35];
extern uint64_t T2_sequence_heads[12];
extern uint64_t T3_sequence_heads[4];
extern uint64_t T4_sequence_heads[15];

//semaphores
extern sem_t semaphores_T1[23];
extern sem_t semaphores_T2[6];
extern sem_t semaphores_T3[2];
extern sem_t semaphores_T4[9];

//threads
extern pthread_t task1_threads[23];
extern pthread_t task2_threads[6];
extern pthread_t task3_threads[2];
extern pthread_t task4_threads[9];

//sequence functions prototypes
void *sequence_1_1_function(void *arguments);
void *sequence_1_2_function(void *arguments);
void *sequence_1_3_function(void *arguments);
void *sequence_1_4_function(void *arguments);
void *sequence_1_5_function(void *arguments);
void *sequence_1_6_function(void *arguments);
void *sequence_1_7_function(void *arguments);
void *sequence_1_8_function(void *arguments);
void *sequence_1_9_function(void *arguments);
void *sequence_1_10_function(void *arguments);
void *sequence_1_11_function(void *arguments);
void *sequence_1_12_function(void *arguments);
void *sequence_1_13_function(void *arguments);
void *sequence_1_14_function(void *arguments);
void *sequence_1_15_function(void *arguments);
void *sequence_1_16_function(void *arguments);
void *sequence_1_17_function(void *arguments);
void *sequence_1_18_function(void *arguments);
void *sequence_1_19_function(void *arguments);
void *sequence_1_20_function(void *arguments);
void *sequence_1_21_function(void *arguments);
void *sequence_1_22_function(void *arguments);
void *sequence_1_23_function(void *arguments);
void *sequence_2_1_function(void *arguments);
void *sequence_2_2_function(void *arguments);
void *sequence_2_3_function(void *arguments);
void *sequence_2_4_function(void *arguments);
void *sequence_2_5_function(void *arguments);
void *sequence_2_6_function(void *arguments);
void *sequence_3_1_function(void *arguments);
void *sequence_3_2_function(void *arguments);
void *sequence_4_1_function(void *arguments);
void *sequence_4_2_function(void *arguments);
void *sequence_4_3_function(void *arguments);
void *sequence_4_4_function(void *arguments);
void *sequence_4_5_function(void *arguments);
void *sequence_4_6_function(void *arguments);
void *sequence_4_7_function(void *arguments);
void *sequence_4_8_function(void *arguments);
void *sequence_4_9_function(void *arguments);
#endif
