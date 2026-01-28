#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>


sem_t prvi,drugi;
pthread_t prva ,druga;
int N,i=1;
int deljivSaSedam;


void*prvaStampa(void*arg){
    
    while(i<=N){
        sem_wait(&prvi);
        sleep(1);
        if(i%7==0&&i<=N){
            printf("Deljiv sa 7: %d\n ",i++);
        }
        sem_post(&drugi);
    }
    sem_post(&drugi);
    return NULL;
}
void*drugaStampa(void*arg){

    
    while(i<=N){
        sem_wait(&drugi);
        while(i%7!=0&&i<=N){
            sleep(1);
            printf("Broj:%d\n ",i++);
        }
        sem_post(&prvi);
    }
    sem_post(&prvi);
    return NULL;
}

int main(int argc,char*argv[]){
    

    if(argc!=2){
        printf("Nema dovoljno arg\n");
        exit(1);
    }



    N=atoi(argv[1]);

    sem_init(&prvi,0,0);
    sem_init(&drugi,0,1);

    pthread_create(&prva,NULL,prvaStampa,NULL);
    pthread_create(&druga,NULL,drugaStampa,NULL);

    pthread_join(prva,NULL);
    pthread_join(druga,NULL);

    sem_destroy(&prvi);
    sem_destroy(&drugi);
    
    
    return 0;
}