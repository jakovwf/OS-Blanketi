#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include "nit1.h"
#include "nit2.h"

sem_t prvi,drugi;
pthread_t prva ,druga;
int N,i=1;
int deljivSaSedam;



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