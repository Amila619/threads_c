#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* routine() 
{
    printf("Test from threads\n");
    sleep(3);
    printf("Ending thread\n");
} 

int main(int argc, char* argv[])
{

    /*
    
    pthread_t *thread → a pointer to store the thread’s ID.
    const pthread_attr_t *attr → thread attributes (usually NULL for default).
    void *(*start_routine)(void *) → the function the thread will run.
    void *arg → argument to pass to the thread function (can be NULL).

    */ 

    pthread_t t1, t2;
    if(pthread_create(&t1, NULL, &routine, NULL) != 0)
    {
        return 1;
    }
    
    if(pthread_create(&t2, NULL, &routine, NULL) != 0)
    {
        return 1;
    }
    
    // wait for threads and get result if needed
    if(pthread_join(t1, NULL) != 0)
    {
        return 2;
    }
    
    if(pthread_join(t2, NULL) != 0)
    {
        return 2;
    }
    
    return 0;
}

/*

    
    void* demo(void* n)
    {
        printf("Starting thread %d\n", *(int*)n);
        sleep(3);
        printf("Thread %d ended\n", *(int*)n);
    }


    int main()
    {
        pthread_t t2;
        int y = 1;

        if(pthread_create(&t2, NULL, &demo, &y) != 0)
        {
            return 2;
        }

        if(pthread_join(t2, NULL) != 0)
        {
            return 1;
        }

        return 0;
    }

*/