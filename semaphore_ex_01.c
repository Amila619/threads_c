#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#define NO_OF_THREADS 4

sem_t semaphore;

void* routine(void* args)
{
    printf("(%d) Waiting in the login queue\n", *(int*)args);
    sem_wait(&semaphore);
    printf("(%d) Logged in\n", *(int*)args);
    sleep(rand() % 5 + 1);
    printf("(%d) Logged out\n", *(int*)args); 
    sem_post(&semaphore);
    free(args);
}

int main()
{
    int i;
    pthread_t th[NO_OF_THREADS];
    sem_init(&semaphore, 0, 2); 
    
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