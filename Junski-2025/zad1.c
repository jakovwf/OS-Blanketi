#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<semaphore.h>
#include<pthread.h>
#include<time.h>

#define SIZE 4

int buffer[SIZE];
int suma=0;

sem_t prvaNit;
sem_t drugaNit;

void* brojevi(void* arg){
    (void)arg;

    while(1){
        sem_wait(&drugaNit);

        if(suma>250){
            break;
        }
        for(int i=0;i<SIZE;i++){
            buffer[i]=rand()%100;
        }
        sem_post(&prvaNit);
    }
    return NULL;

}
int main(void){
    sem_init(&drugaNit, 0,1);
    sem_init(&prvaNit, 0,0);

    pthread_t nit;
    pthread_create(&nit, NULL, brojevi, NULL);

    while(1){
        sem_wait(&prvaNit);
        suma=0;

        for(int i=0;i<SIZE;i++)
            suma+=buffer[i];
            if(suma<50)
                printf("Suma je manja od 50 %d", suma);
            else
            printf("Suma je veca od 50 %d", suma);
        
        if(suma>250){
            sem_post(&drugaNit);
            break;
        }
        sem_post(&drugaNit);
    }
    pthread_join(nit, NULL);
    sem_destroy(&drugaNit);
    sem_destroy(&prvaNit);
    return 0;
}