#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#define NO_OF_THREADS 4

/*

    A semaphore is a synchronization tool used to control access to shared resources by multiple threads.
    Think of it like a traffic light or a counter that limits how many threads can enter a critical section at the same time.
    Semaphores prevent race conditions and manage concurrent access.

    1️⃣ Types of Semaphores

    - Binary Semaphore
        Value: 0 or 1
        Works like a mutex (locks/unlocks)
        Only one thread can enter the critical section at a time

    - Counting Semaphore
        Value: 0, 1, 2, …
        Controls access to a resource with multiple units (e.g., a buffer with N slots)


    Operations:
        - sem_wait(&sem) → wait / decrement / block if 0
        - sem_post(&sem) → signal / increment / release

    Usage: Protect critical sections, limit concurrent threads, prevent race conditions.

    | Feature                        | Mutex                                  | Semaphore                          |
    | ------------------------------ | -------------------------------------- | ---------------------------------- |
    | Access                         | Exclusive (1 thread)                   | Can be multiple (depends on count) |
    | Value                          | Binary (locked/unlocked)               | Integer (0 to N)                   |
    | Ownership                      | Thread that locks must unlock          | Any thread can signal              |
    | Use Case                       | Critical section protection            | Resource counting, signaling       |
    | Can Be Shared Across Processes | Usually no (depends on implementation) | Yes, can be used across processes  |

    Mutex: Only one thread can use the resource at a time. The thread that locks it must unlock it.
    Semaphore: Allows multiple threads (up to a set limit) to use the resource. Any thread can release it.

    Easy analogy:
        - Mutex = bathroom key — only one person can go in at a time.
        - Semaphore = tickets for a ride — a few people can go in together until tickets run out.

    sem_post() is the signal, and sem_wait() is the wait.

*/ 

sem_t semaphore;

void* routine(void* args)
{
    sem_wait(&semaphore);
    sleep(1);
    printf("Hello from thread %d\n", *(int*)args);
    sem_post(&semaphore);
    free(args);
}

int main()
{
    int i;
    pthread_t th[NO_OF_THREADS];
    sem_init(&semaphore, 0, 1); 
    // second param : if multiple processes then 1 else 0
    // third param : value stored (if 2 then 2 therads run)
    
    for(i = 0 ; i < NO_OF_THREADS ; i++)
    {
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i], NULL, &routine, a) != 0 )
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

    sem_destroy(&semaphore);
    return 0;
}