#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<string.h>

#define LEN 256

sem_t sem1, sem2;
FILE* izlazna;
int kraj=0;
int zavrsila=0;

void* citajizPrve(void* arg){
    char* imeFajla=(char*)arg;

    FILE* f=fopen(imeFajla, "r");

    if(f==0){
        exit(1);
    }
    char linija[LEN];
    while(fgets(linija, LEN, f)!=NULL){
        sem_wait(&sem1);

        if(kraj){
            break;
        }
        fprintf(izlazna, "%s:%s ", imeFajla, linija);
        sem_post(&sem2);
    }
    zavrsila++;
    if(zavrsila==2){
        kraj=1;
    }
    sem_post(&sem2);
    fclose(f);
}
void* citajizDruge(void* arg){
    char* ImeFajla=(char*)arg;
    FILE* f=fopen(ImeFajla, "r");

    if(f==0){
        exit(1);
    }
    char linija[LEN];
    while(fgets(linija, LEN, f)!=NULL){
        sem_wait(&sem2);
        if(kraj){
            break;
        }
        fprintf(izlazna, "%s:%s", ImeFajla, linija);
        sem_post(&sem1);
    }
    zavrsila++;
    if(zavrsila==2){
        kraj=1;
    }
    sem_post(&sem1);
    fclose(f);

}
int main(int argc, char* argv[]){
    if(argc!=4){
        printf("Nedovoljan broj argumenata");
        exit(1);
    }
    izlazna=fopen(argv[3], "w");
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

    pthread_t nit1, nit2;

    pthread_create(&nit1, NULL, (void*)citajizPrve, (void*)argv[1]);
    pthread_create(&nit2, NULL, (void*)citajizDruge, (void*)argv[2]);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    fclose(izlazna);
    return 0;
}