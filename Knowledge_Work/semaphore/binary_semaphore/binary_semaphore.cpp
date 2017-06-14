#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
sem_t semP, semC;
int stock_count = 0;
const int stock_max_limit=5;

void *producer(void *arg) {

    int i, sum=0;
    for (i = 0; i < 10; i++) {

      printf("producer::stock counter : %d and i is : %d \n", stock_count, i);
        while(stock_max_limit == stock_count){
            printf("producer wait \n");
            sem_wait(&semC);
        }

        sleep(1);   //production decided here
        stock_count++;
        sem_post(&semP);
    }
 }

void *consumer(void *arg) {
    int i, sum=0;
    for (i = 0; i < 10; i++) {

      printf("consumer::stock counter : %d and i is %d \n", stock_count, i);
        while(0 == stock_count){
            printf("consumer wait \n");
            sem_wait(&semP);
        }

        sleep(2);   //consumer rate decided here
        stock_count--;
        sem_post(&semC);
        }
}

int main(void) {

    pthread_t tid0,tid1;
    sem_init(&semP, 0, 0);
    sem_init(&semC, 0, 0);

        pthread_create(&tid0, NULL, consumer, NULL);
        pthread_create(&tid1, NULL, producer, NULL);
        pthread_join(tid0, NULL);
        pthread_join(tid1, NULL);

    sem_destroy(&semC);
    sem_destroy(&semP);

    return 0;
}
/* output :

producer::stock counter : 0 and i is : 0 
consumer::stock counter : 0 and i is 0 
consumer wait 
producer::stock counter : 1 and i is : 1 
producer::stock counter : 2 and i is : 2 
consumer::stock counter : 1 and i is 1 
producer::stock counter : 2 and i is : 3 
producer::stock counter : 3 and i is : 4 
consumer::stock counter : 2 and i is 2 
producer::stock counter : 3 and i is : 5 
producer::stock counter : 4 and i is : 6 
consumer::stock counter : 3 and i is 3 
producer::stock counter : 4 and i is : 7 
producer::stock counter : 5 and i is : 8 
producer wait 
producer wait 
producer wait 
producer wait 
consumer::stock counter : 4 and i is 4 
producer::stock counter : 5 and i is : 9 
producer wait 
consumer::stock counter : 4 and i is 5 
consumer::stock counter : 4 and i is 6 
consumer::stock counter : 3 and i is 7 
consumer::stock counter : 2 and i is 8 
consumer::stock counter : 1 and i is 9 

*/
