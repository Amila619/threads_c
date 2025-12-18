#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;

void* routine() 
{
    int i;
    for(i = 0 ; i < 100000 ; i++)
    {
        mails++;
        // read mails
        // increment 
        // write mails
    }
} 

int main(int argc, char* argv[])
{

    pthread_t t1, t2;
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
    
    printf("No of mails : %d\n", mails);

    return 0;
}

/*

Race Condition - A race condition is when two or more threads or processes 
try to access and modify the same data at the same time, 
and the final result depends on who finishes first.

“Two things are racing to change the same thing, 
and the winner decides the outcome.”

No of mails : 183127
No of mails : 113865
No of mails : 109011
No of mails : 143669

gcc -S race_condition.c

A race condition can only occur on a multi-core processor

*/ 