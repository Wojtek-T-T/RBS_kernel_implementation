#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>

#define WAIT 455
#define INIT_RBS 452
#define INIT_TASK 453
#define REL_JOB 454
#define TRY_EX 456
#define EXECUTED 457

#define TASK_1 1


void *thread_func(void *arguments);
void *thread_func2(void *arguments);
void *thread_func3(void *arguments);

u_int32_t arrayh[5] = {0, 1, 1, 1, 14};
u_int32_t arrayv[5] = {14, 16, 16, 16, 0};
u_int32_t arraysh[5] = {1, 0, 2, 3, 0};

pthread_t test_thread;


int main (int argc, char **argv)
{
        int ret = syscall(INIT_RBS);
        printf("rbs init %d\n", ret);
        ret = syscall(INIT_TASK, TASK_1, 3, 2);
        printf("task init %d\n", ret);

        pthread_create(&test_thread, NULL, &thread_func, NULL);

        sleep(2);

        ret = syscall(REL_JOB, TASK_1);
        printf("job released %d\n", ret);

        sleep(5);

        return 0;
}




void *thread_func(void *arguments)
{
    //printf("waiting on guard\n");
    int ret = syscall(WAIT, TASK_1, 1);
    //printf("guard released %d\n", ret);

     ret = syscall(TRY_EX, TASK_1, 1, 1);
     if(ret != 0)
     {
        return 0;
     }
    sleep(1);
     ret = syscall(EXECUTED, TASK_1, 1, 1);
     printf("EXECUTED NODE %d, SEQUENCE %d", 1, 1);


     ret = syscall(TRY_EX, TASK_1, 1, 2);
    if(ret != 0)
     {
        return 0;
     }
    sleep(1);
     ret = syscall(EXECUTED, TASK_1, 1, 2);
     printf("EXECUTED NODE %d, SEQUENCE %d", 2, 1);


     ret = syscall(TRY_EX, TASK_1, 1, 5);
    if(ret != 0)
     {
        return 0;
     }
    sleep(1);
     ret = syscall(EXECUTED, TASK_1, 1, 5);
     printf("EXECUTED NODE %d, SEQUENCE %d", 5, 1);

    return 0;
}

void *thread_func2(void *arguments)
{
    //printf("waiting on guard\n");
    int ret = syscall(WAIT, TASK_1, 2);
    //printf("guard released %d\n", ret);

     
     ret = syscall(TRY_EX, TASK_1, 2, 3);
     if(ret != 0)
     {
        return 0;
     }
    sleep(1);
     ret = syscall(EXECUTED, TASK_1, 2, 3);
     printf("EXECUTED NODE %d, SEQUENCE %d", 3, 2);


     ret = syscall(TRY_EX, TASK_1, 2, 5);
    if(ret != 0)
     {
        return 0;
     }
    sleep(1);
     ret = syscall(EXECUTED, TASK_1, 2, 5);
     printf("EXECUTED NODE %d, SEQUENCE %d", 5, 2);


    return 0;
}


void *thread_func3(void *arguments)
{
   // printf("waiting on guard\n");
    int ret = syscall(WAIT, TASK_1, 3);
   // printf("guard released %d\n", ret);

     ret = syscall(TRY_EX, TASK_1, 3, 4);
     if(ret != 0)
     {
        return 0;
     }
    sleep(1);
     ret = syscall(EXECUTED, TASK_1, 3, 4);
     printf("EXECUTED NODE %d, SEQUENCE %d", 4, 3);


     ret = syscall(TRY_EX, TASK_1, 3, 5);
    if(ret != 0)
     {
        return 0;
     }
    sleep(1);
     ret = syscall(EXECUTED, TASK_1, 3, 5);
     printf("EXECUTED NODE %d, SEQUENCE %d", 5, 3);

    return 0;
}
