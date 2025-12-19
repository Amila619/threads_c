#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#define NO_OF_THREADS 2

/*

    Pthread Barrier (Simple Note)

    - Purpose: Synchronize threads so that all threads wait at a point before continuing.
    - Data type: pthread_barrier_t

    Functions
    
    1)Initialize barrier
    pthread_barrier_init(&barrier, NULL, count); // count = number of threads to wait
    
    2)Wait at barrier
    pthread_barrier_wait(&barrier); // blocks until all threads reach here
    
    3)Destroy barrier
    pthread_barrier_destroy(&barrier);
    
    Example
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, 3);
    void* threadFunc(void* arg) {
        printf("Phase 1 done\n");
        pthread_barrier_wait(&barrier);
        printf("Phase 2 starts\n");
        return NULL;
    }

    Behavior: All threads finish Phase 1, then start Phase 2 together.
    Key Points:

    Blocks threads until all arrive.
    One thread may return PTHREAD_BARRIER_SERIAL_THREAD (optional “leader”).
    Must destroy barrier after use.

*/ 

pthread_barrier_t barrier;

void* routine(void* args)
{
    printf("Waiting at the barrier...\n");    
    sleep(1);
    pthread_barrier_wait(&barrier);
    printf("We passed the barrier...\n");
}

int main()
{
    int i;
    pthread_t th[NO_OF_THREADS];
    pthread_barrier_init(&barrier, NULL, NO_OF_THREADS); 

    for(i = 0 ; i < NO_OF_THREADS ; i++)
    {
        if(pthread_create(&th[i], NULL, &routine, NULL) != 0 )
        {
            perror("Failed to create thread");
        }
    }

    for(i = 0 ; i < NO_OF_THREADS ; i++)
    {
        if(pthread_join(th[i], NULL) != 0 )
        {
            perror("Failed to join thread");
        }
    }

    return 0;
}