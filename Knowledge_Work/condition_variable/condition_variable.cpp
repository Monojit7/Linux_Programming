/*

condition variable is a system object which is used for synchronization between two threads when one thread wait for a 
particular resources which will be updated by some other thread. 

When the condition occurs the other thread sends a signal to the waiting thread and which unblocks the thread

Ex:

T1 thread waiting for a1 to available ( pthread_cond_wait)

T2 gets the information about a1 availability and it sends a1 is available
through pthread_cond_signal to T1.

T1 wakes up and executes the rest

*/


#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex_t = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_t = PTHREAD_COND_INITIALIZER;

int condition = 0;

void* consumer(void* arg)
{

printf("consumer thread \n");
pthread_mutex_lock(&mutex_t);
printf("waiting in the condition variable: %d\n", condition);
pthread_cond_wait(&cond_t, &mutex_t);// pthread_cond_wait: It releases the mutex lock and make the thread block or waiting under condition variable
printf("consumer thread ended \n");

}


void* producer(void*)
{

printf("producer thread \n");

while(condition < 20)
{
if(condition == 10 )
{
printf("Signal to consumer thread \n");
pthread_cond_signal(&cond_t); // pthread_cond_signal: It unblocks atleast one thread which blocks under condition variable
                              // pthread_cond_broadcast: It unblocks all the thread which blocks under conditional variable


}

printf("The value of the condition : %d \n", ++condition);

}


}


int main()
{

pthread_t tid[2];
pthread_create(&tid[0], NULL, &producer,NULL);
 pthread_create(&tid[1], NULL, &consumer, NULL);

pthread_join(tid[0], NULL);
pthread_join(tid[1], NULL); 

return 0;
}

/*

output:
consumer thread 
waiting in the condition variable: 0
producer thread 
The value of the condition : 1 
The value of the condition : 2 
The value of the condition : 3 
The value of the condition : 4 
The value of the condition : 5 
The value of the condition : 6 
The value of the condition : 7 
The value of the condition : 8 
The value of the condition : 9 
The value of the condition : 10 
Signal to consumer thread 
The value of the condition : 11 
The value of the condition : 12 
The value of the condition : 13 
The value of the condition : 14 
The value of the condition : 15 
The value of the condition : 16 
The value of the condition : 17 
The value of the condition : 18 
The value of the condition : 19 
The value of the condition : 20 
consumer thread ended 

*/
