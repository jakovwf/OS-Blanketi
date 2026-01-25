#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>

#define LEN 256

sem_t parni, neparni;
int kraj;

void* citajizPrve(void* arg){
    char* ImeFajla=(char*)arg;

    FILE *f=fopen(ImeFajla, "r");
    if(f==0){
        exit(1);
    }
    char linija[LEN];
    int brlinije=1;
    while(fgets(linija, LEN, f)!=NULL){
        if(brlinije%2==0){
        
        sem_wait(&parni);
        if(kraj){
            break;
        }
        printf("%s", linija);
        FILE *f1=fopen("zbir.txt", "a");
        fprintf(f1, "%s(linija %d):%s", ImeFajla, brlinije, linija);
        fclose(f1);
         sem_post(&neparni);
    }
        brlinije++;
}
    kraj=1;
    sem_post(&neparni);
    fclose(f);
}
void* citajDrugu(void* arg){
    char* imeFajla=(char*)arg;

    FILE *f=fopen(imeFajla,"r");

    if(f==0){
        exit(1);
    }
    char linija[LEN];
    int brlinije=1;
    while(fgets(linija, LEN, f)!=NULL){
        if(brlinije%2==1){
        sem_wait(&neparni);

        if(kraj){
            break;
        }
        printf("%s", linija);

        FILE*f1=fopen("zbir.txt", "a");

        fprintf(f1, "%s (linija %d): %s", imeFajla, brlinije, linija);

        fclose(f1);
        sem_post(&parni);
    }

        brlinije++;
    }
    kraj=1;
    sem_post(&parni);
    fclose(f);
}
int main(int argc, char* argv[]){

    if(argc!=3){
        printf("Neodgovarajuci broj argumenata");
        exit(1);
    }

    sem_init(&parni, 0, 0);
    sem_init(&neparni, 0, 1);

    pthread_t nit1,nit2;

    pthread_create(&nit1, NULL, (void*)citajizPrve, (void*)argv[1]);
    pthread_create(&nit2, NULL, (void*)citajDrugu, (void*)argv[2]);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    sem_destroy(&parni);
    sem_destroy(&neparni);

    return 0;

}
