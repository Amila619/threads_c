#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#define NO_OF_THREADS 2

/*

    1)Manage shared memory access
    2)Checking for if buffer is full
    3)Checking for if buffer is emptry

*/ 

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;

void* producer(void* args)
{
    while(1)
    {
        // Produce
        int x = rand() % 100;
        // sleep(1);
        
        // Add to the Buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = x;
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* args)
{
    while(1)
    {    
        int y = -1;


        sem_wait(&semFull);
        // Remove from the buffer
        pthread_mutex_lock(&mutexBuffer);
        y = buffer[count - 1];
        count--;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        
        // Consume
        printf("Got %d\n", y);
        sleep(1);
    }
}

int main()
{
    int i;
    pthread_t th[2];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);

    for( i = 0 ; i < NO_OF_THREADS ; i++)
    {
        if(i % 2 == 0)
        {
            if(pthread_create(&th[i], NULL, &producer, NULL) != 0 )
            {
                perror("Failed to create thread");
            }
        }else
        {
            if(pthread_create(&th[i], NULL, &consumer, NULL) != 0 )
            {
                perror("Failed to create thread");
            }
        }
    }

    for( i = 0 ; i < NO_OF_THREADS ; i++)
    {
        if(pthread_join(th[i], NULL) != 0)
        {
            perror("Failed at joining thread");
        }
    }

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}