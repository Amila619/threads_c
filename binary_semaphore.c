#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

/*

    A binary semaphore is a synchronization tool that can have only two values: 0 or 1.
        0 → resource is unavailable (locked)
        1 → resource is available (unlocked)

    It is mainly used to control access to a shared resource and prevent race conditions between threads or processes.

    Key points:
        Works like a lock but is managed by the operating system
        Any thread can signal (release) the semaphore
        Commonly used for mutual exclusion and event signaling

    Example use:
    Ensuring only one thread accesses a critical section at a time.

    👉 In simple terms: a binary semaphore allows only one “permit” at a time.

*/ 

#define THREAD_NUM 1

sem_t semFuel;
pthread_mutex_t mutexFuel;

int *fuel;

void* routine(void* args) {
    *fuel += 50;
    printf("Current value is %d\n", *fuel);
    sem_post(&semFuel);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    fuel = malloc(sizeof(int));
    *fuel = 50;
    pthread_mutex_init(&mutexFuel, NULL);
    sem_init(&semFuel, 0, 0);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
    }
    sem_wait(&semFuel);
    printf("Deallocating fuel\n");
    free(fuel);

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    sem_destroy(&semFuel);
    return 0;
}