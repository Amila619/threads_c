#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*
    A mutex is like a lock — only one thread can access the shared resource at a time.
    Before a thread uses shared data, it locks the mutex.
    After it’s done, it unlocks the mutex, allowing other threads to access the data safely.
    It is used to prevent race conditions.

    - Critical Section: mails++ is the part that must be protected.
    - Race Condition Avoided: The mutex ensures only one thread modifies mails at a time.
    - Thread Safety: Always lock before accessing shared resources and unlock after done.

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

    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);

    if(pthread_create(&t1, NULL, &routine, NULL) != 0)
    {
        return 1;
    }
    
    if(pthread_create(&t2, NULL, &routine, NULL) != 0)
    {
        return 1;
    }
    
    // wait for threads
    if(pthread_join(t1, NULL) != 0)
    {
        return 2;
    }
    
    if(pthread_join(t2, NULL) != 0)
    {
        return 2;
    }
    
    pthread_mutex_destroy(&mutex);
    printf("No of mails : %d\n", mails);

    return 0;
}