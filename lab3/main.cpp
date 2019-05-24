////
////#include <sys/types.h>
////#include <unistd.h>
////#include <stdio.h>
////#include <stdlib.h>
////#include <pthread.h>
////#include <fcntl.h>
////#include <sys/stat.h>
////#include <semaphore.h>
//////sem_t* mySem_1;
////sem_t* mySem_2;
//////sem_t* mySem_3;
////sem_t* mySem_4_2;
////sem_t* mySem_4_3;
////void print_Infomation(int id){
////    printf("I am the process P%d \n",id);
////}
////
////int main(){
////    //mySem_1 = sem_open("1", O_CREAT, 0666, 1);
////    mySem_2 = sem_open("2", O_CREAT, 0666, 0);
////    //mySem_3 = sem_open("3", O_CREAT, 0666, 1);
////    mySem_4_2 = sem_open("4_2", O_CREAT, 0666, 0);
////    mySem_4_3 = sem_open("4_3", O_CREAT, 0666, 0);
////    pid_t p1,p2,p3,p4;
////
////    //p1 = fork();p2=fork();p3=fork();p4=fork();
////
////    if((p1 = fork())<0){
////        perror("p1 error");
////    }else if((p2 = fork())<0){
////        perror("p2 error");
////    }else if((p3 = fork())<0){
////        perror("p3 error");
////    }else if((p4 = fork())<0){
////        perror("p4 error");
////    }else{
////        //printf("%d %d %d %d\n",p1,p2,p3,p4);
////        //sem_post(mySem_1);
////        if(p1==0){
////            //sem_wait(mySem_1);
////            print_Infomation(4);
////            sem_post(mySem_2);
////            exit(0);
////        }else{
////            if(p2==0||p3==0){
////            if(p2==0){
////                //sleep(1);
////                sem_wait(mySem_2);
////                print_Infomation(3);
////                sem_post(mySem_2);sem_post(mySem_4_2);
////                exit(0);
////            }
////            if(p3==0){
////                //sleep(1);
////                sem_wait(mySem_2);
////                print_Infomation(2);
////                sem_post(mySem_2);sem_post(mySem_4_3);
////                exit(0);
////            }
////            }else if(p4==0){
////            if(p4==0){
////                sem_wait(mySem_4_2);sem_wait(mySem_4_3);
////                print_Infomation(1);
////                exit(0);
////            }
////            }
////            exit(0);
////        }
////    }
////
////    //sem_close(mySem_1);
////    sem_close(mySem_2);
////    //sem_close(mySem_3);
////
////    sem_close(mySem_4_2);
////    sem_close(mySem_4_3);
////    //sem_unlink("1");
////    sem_unlink("2");
////    //sem_unlink("3");
////
////    sem_unlink("4_2");
////    sem_unlink("4_3");
////    return 0;
////}
//
//#include<sys/types.h>
//#include<unistd.h>
//#include<stdio.h>
//#include<stdlib.h>
//#include<pthread.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#include <semaphore.h>
//#include <sched.h>
//sem_t* read_mySem = NULL;
//sem_t* print_mySem = NULL;
//
//char buf[10];
//short i = 0;
//short j = 0;
//
//void*read_char(){
//    while(1){
//        sem_wait(read_mySem);
//        char *t = &buf[i++%10];
//        scanf("%c",t);
//        sem_post(print_mySem);
//        // if(*t == '\n') break;
//        
//    }
//}
//
//void*print_char(){
//    while(1){
//        sleep(1);
//        sem_wait(print_mySem);
//        char *t = &buf[j++%10];
//        // if(*t == '\n') break;
//        printf("%d:%c\n",j,*t);
//        sem_post(read_mySem);
//    }
//}
//
//int main(){
//    pthread_t p1,p2;
//    
//    read_mySem = sem_open("read_chars", O_CREAT, 0666, 10);
//    print_mySem = sem_open("print_chars", O_CREAT, 0666, 0);
//    
//    pthread_create(&p1,NULL,read_char,NULL);
//    pthread_create(&p2,NULL,print_char,NULL);
//    
//    pthread_join(p1,NULL);
//    pthread_join(p2,NULL);
//    
//    sem_close(read_mySem);
//    sem_close(print_mySem);
//    
//    sem_unlink("read_chars");
//    sem_unlink("print_chars");
//    return 0;
//}

