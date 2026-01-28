#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

#define N 10
#define brojponavljanja 5
int buffer[N];
sem_t stampa, proverava;
void* stampajNaParne(void* arg){
    
    for(int i=0;i<brojponavljanja;i++){
        sem_wait(&stampa);

        for(int j=0;j<N;j+=2){
            buffer[j]=rand()%300;
        }
        sem_post(&proverava);
    }
    return NULL;
}
void* stampajNaNeparne(void* arg){
    for(int i=0;i<brojponavljanja;i++){
        sem_wait(&stampa);
        for(int j=1;j<N;j+=2){
            buffer[j]=(rand()%200)+300;
        }
        sem_post(&proverava);
    }
    return NULL;
}
int main(){

    sem_init(&proverava, 0, 0);
    sem_init(&stampa, 0 ,2);
    pthread_t nit1, nit2;

    pthread_create(&nit1, NULL, (void*)stampajNaParne, NULL);
    pthread_create(&nit2, NULL, (void*)stampajNaNeparne, NULL);

    for(int i=0;i<brojponavljanja;i++){
        sem_wait(&proverava);
        sem_wait(&proverava);
        int zbir=0;
        for(int j=0;j<N;j++){
        zbir+=buffer[j];
        }

        if(zbir>2000){
            printf("Zbir brojeva je veci od 2000: %d\n", zbir);
        }
        else{
            printf("Zbir brojeva je manji od 2000: %d\n", zbir);
        }
        sem_post(&stampa);
        sem_post(&stampa);
    }
    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    sem_destroy(&stampa);
    sem_destroy(&proverava);

    return 0;
}
