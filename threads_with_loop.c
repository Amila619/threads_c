#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define  NO_OF_THREADS 4

/*
    A mutex is like a lock — only one thread can access the shared resource at a time.
    Before a thread uses shared data, it locks the mutex.
    After it’s done, it unlocks the mutex, allowing other threads to access the data safely.
    It is used to prevent race conditions.

    - Critical Section: mails++ is the part that must be protected.
    - Race Condition Avoided: The mutex ensures only one thread modifies mails at a time.
    - Thread Safety: Always lock before accessing shared resources and unlock after done.

    “A mutex blocks threads from entering the same protected code simultaneously.”

    A recursive mutex is like a key that the same person can reuse multiple times before returning it 
    — but they must return it the same number of times.

*/

int mails = 0;
pthread_mutex_t mutex;

void* routine() 
{
    int i;
    for(i = 0 ; i < 100000 ; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
} 

int main(int argc, char* argv[])
{

    int i;
    pthread_t th[NO_OF_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (i = 0 ; i < NO_OF_THREADS ; i++)
    {
        if(pthread_create(&th[i], NULL, &routine, NULL) != 0)
        {
            return 1;
        }    
    }

    for (i = 0 ; i < NO_OF_THREADS ; i++)
    {    
        if(pthread_join(th[i], NULL) != 0)
        {
            return 2;
        }
    }
    
    pthread_mutex_destroy(&mutex);
    printf("No of mails : %d\n", mails);

    return 0;
}