#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define  NO_OF_THREADS 4

/*

    pthread_mutex_lock → “Wait in line until the bathroom is free.”
    pthread_mutex_trylock → “Check if the bathroom is free. If not, go do something else.”

    | Feature                | `pthread_mutex_lock`        | `pthread_mutex_trylock`     |
    | ---------------------- | --------------------------- | --------------------------- |
    | Block if locked        | ✅ Yes                      | ❌ No, returns immediately |
    | Return value if locked | waits (no return yet)       | returns `EBUSY`             |
    | Use case               | Must enter critical section | Try, but don’t wait         |


*/

pthread_mutex_t mutex;

void* routine(void* arg) 
{
        // if 0 you got the lock
        if(pthread_mutex_trylock(&mutex) == 0)
        {
            printf("Got lock\n");
            sleep(1);
            pthread_mutex_unlock(&mutex);
        }else
        {
            printf("Didn't get lock\n");
        }

        // pthread_mutex_lock(&mutex);
        // printf("Got lock\n");
        // sleep(1);
        // pthread_mutex_unlock(&mutex);
    
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
            perror("Error at creating thread");
        }    
    }

    for (i = 0 ; i < NO_OF_THREADS ; i++)
    {    
        if(pthread_join(th[i], NULL) != 0)
        {
            perror("Error at joining thread");
        }
    }
    
    pthread_mutex_destroy(&mutex);

    return 0;
}