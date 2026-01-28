#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>


sem_t prvi,drugi;
pthread_t prva ,druga;
int N,i=1,kraj=0;
int deljivSaSedam;


void*prvaStampa(void*arg){
    

    while(1){
        sem_wait(&prvi);
    
        if(i%7==0){
            printf("Deljiv sa 7: %d\n ",i++);
        }
        if(i==N||kraj==1){
            kraj=1;
            sem_post(&drugi);
            break;
        }
        sem_post(&drugi);
        
    }
    return NULL;
}
void*drugaStampa(void*arg){

    
    while(kraj!=1){
        sem_wait(&drugi);
        while(i%7!=0&&i<N){
            printf("Broj:%d\n ",i++);
        }
        if(i==N){
            kraj=1;
            sem_post(&prvi);
            break;
        }
        sem_post(&prvi);
    }
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