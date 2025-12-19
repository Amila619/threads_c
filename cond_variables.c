#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#define NO_OF_THREADS 2

/*

    🟢 What is a condition variable?

    A condition variable is a synchronization tool that allows threads to wait for a certain condition to become true before continuing.
    It’s used together with a mutex.
    Threads can wait for a condition.
    Threads can signal when the condition happens so other threads wake up.
    Think of it as a “wait queue” for threads.

    🟢 Why use it?
        Sometimes a mutex isn’t enough.
        Example:
            A thread produces data
            Another thread consumes data
            The consumer should wait until there is data, instead of constantly checking (busy-waiting)
        Condition variables solve this efficiently.

    | Function                 | Description                                 |
    | ------------------------ | ------------------------------------------- |
    | `pthread_cond_init`      | Initialize a condition variable             |
    | `pthread_cond_wait`      | Wait on a condition (blocks until signaled) |
    | `pthread_cond_signal`    | Wake up **one** waiting thread              |
    | `pthread_cond_broadcast` | Wake up **all** waiting threads             |
    | `pthread_cond_destroy`   | Destroy the condition variable              |


*/

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

int fuel = 0;
int i;

void* fuel_filling(void* arg) 
{
    for(i = 0 ; i < 5 ; i++)
    {
        pthread_mutex_lock(&mutexFuel);
        fuel += 15;
        printf("Filling fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        pthread_cond_signal(&condFuel);
        sleep(1);
    }
} 

void* car(void* arg) 
{
    pthread_mutex_lock(&mutexFuel);
    while(fuel < 40)
    {
        printf("No fuel. waiting...\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
        // pthread_mutex_unlock(&mutexFuel);
        // wait for signal on condFuel
        // pthread_mutex_lock(&mutexFuel)
    }
    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
} 

int main(int argc, char* argv[])
{

    int i;
    pthread_t th[NO_OF_THREADS];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);


    for (i = 0 ; i < NO_OF_THREADS ; i++)
    {
        if(i == 1)
        {
            if(pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0)
            {
                perror("Error at creating thread");
            }
        }else
        {
            if(pthread_create(&th[i], NULL, &car, NULL) != 0)
            {
                perror("Error at creating thread");
            }
        }    
    }

    for (i = 0 ; i < NO_OF_THREADS ; i++)
    {    
        if(pthread_join(th[i], NULL) != 0)
        {
            perror("Error at joining thread");
        }
    }

    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    
    return 0;
}