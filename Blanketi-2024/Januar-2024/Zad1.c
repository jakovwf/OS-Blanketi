#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#include <string.h>
#define LEN 256

pthread_t nit1, nit2;
sem_t prvaDat, drugaDat;

typedef struct{
    char ulazni[20];
    char izlazni[20];
}Fajl;

int kraj=0;

void* citajizPrve(void* arg){
    Fajl * fajl1=(Fajl*)arg;
    
    FILE*f=fopen(fajl1->ulazni,"r");

    if(f==0){
        exit(1);
    }

    char linija[LEN];
    while(fgets(linija, LEN, f)!=NULL){
        sem_wait(&prvaDat);
        if(kraj){
            break;
        }
        FILE* f1=fopen(fajl1->izlazni,"a");
        fprintf(f1, "%s:%s", fajl1->ulazni, linija);
        fclose(f1);
        sem_post(&drugaDat);
    }
    fclose(f);
    sem_post(&drugaDat);
    kraj=1;
}

void* citajizDruge(void* arg){
    Fajl * fajl1 = (Fajl*)arg;
    
    FILE*f=fopen(fajl1->ulazni, "r");
    if(f==0){
        exit(1);
    }

    char linija[LEN];
    while(fgets(linija, LEN, f)!=NULL){
        sem_wait(&drugaDat);

        if(kraj){
            break;
        }
        FILE*f1=fopen(fajl1->izlazni, "a");
        fprintf(f1, "%s:%s", fajl1->ulazni, linija);
        fclose(f1);
        sem_post(&prvaDat);
    }
    fclose(f);
    sem_post(&prvaDat);
}
int main(int argc, char* argv[]){
    
    if(argc!=4){
        printf("Neodgovarajuci broj elemenata");
        exit(1);
    }

    Fajl fileNit1;
    Fajl fileNit2;
    strcpy(fileNit1.ulazni,argv[1]);
    strcpy(fileNit2.ulazni,argv[2]);
    strcpy(fileNit1.izlazni,argv[3]);
    strcpy(fileNit2.izlazni,argv[3]);
    
    
    sem_init(&prvaDat, 0,1);
    sem_init(&drugaDat, 0,0);

    pthread_create(&nit1, NULL, (void*)citajizPrve, (void*)&fileNit1);
    pthread_create(&nit2,NULL, (void*)citajizDruge, (void*)&fileNit2);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);
    sem_destroy(&prvaDat);
    sem_destroy(&drugaDat);

    return 0;
}
