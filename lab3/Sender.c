#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
pthread_mutex_t mutex ;
int ticket_counter;
int producer_counter;
int consumer_counter;
int all_pro;int all_con;

void *producer( int i ) {
    //printf("this is producer");
    while(--i>0){
        pthread_mutex_lock( &mutex );
        //sched_yield();
        ticket_counter++;
        sched_yield();
        sched_yield();
        pthread_mutex_unlock( &mutex );
    }
    pthread_exit( NULL );
}

void *consumer( int j ) {
    while(j-->0){
        pthread_mutex_lock( &mutex );
        //sched_yield();
        ticket_counter--;
        sched_yield();
        sched_yield();
        pthread_mutex_unlock( &mutex );
    }
    pthread_exit( NULL );
}

void create_and_join_Pthread(){
    all_con=0;all_pro=0;
    pthread_t *pro, *con;
    pro=(pthread_t*)malloc(sizeof(pthread_t)*producer_counter);
    con=(pthread_t*)malloc(sizeof(pthread_t)*consumer_counter);
    printf("[%d]个线程分别产生的票数为: ",producer_counter);
    srand((unsigned)time(NULL));
    for(int i=0;i<producer_counter;i++){
        int a = rand() % 51 + 1;
        all_pro+=a;
        printf("%d ",a);
        pthread_create( &pro[i], NULL, producer, a );
    }
    printf("\n");
    printf("[%d]个线程分别售出的票数为: ",consumer_counter);
    for(int i=0;i<consumer_counter;i++){
        int a = rand() % 51 + 1;
        all_con+=a;
        printf("%d ",a);
        pthread_create( &con[i], NULL, consumer, a );
    }
    printf("\n");
    for(int i=0;i<producer_counter;i++){
        pthread_join( pro[i], NULL );
    }
    for(int i=0;i<producer_counter;i++){
        pthread_join( con[i], NULL );
    }
}

int main() {
    printf("输入产生车票线程数/销售车票线程数/开始时总票数:");
    scanf("%d %d %d",&producer_counter,&consumer_counter,&ticket_counter);
//    ticket_counter=10000;
//    producer_counter=12;
//    consumer_counter=2;
    int temp=ticket_counter;
    pthread_mutex_init(&mutex, NULL);
    create_and_join_Pthread();
    printf("完成该过程后应有余票[%d],其中售出[%d],生产[%d].\n",temp-all_con+all_pro,all_con,all_pro);
    printf("\n实际还有余票为[%d]\n",ticket_counter+producer_counter);
    return 0;
}


