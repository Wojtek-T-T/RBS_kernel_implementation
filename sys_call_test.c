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


void *thread_func(void *arguments);

u_int32_t array1[3] = {1,2,3};
u_int32_t array2[3] = {4,5,6};
u_int32_t array3[2] = {1,2};

pthread_t test_thread;


int main (int argc, char **argv)
{
        int ret = syscall(452);
        printf("rbs init %d\n", ret);
        ret = syscall(453, 1, 3, 2);
        printf("task init %d\n", ret);

        pthread_create(&test_thread, NULL, &thread_func, NULL);

        sleep(2);

        ret = syscall(454, 1);
        printf("job released %d\n", ret);

        sleep(5);

        return 0;
}




void *thread_func(void *arguments)
{
    printf("waiting on guard\n");
    int ret = syscall(455, 1, 1);
    printf("guard released %d\n", ret);

    return 0;
}
