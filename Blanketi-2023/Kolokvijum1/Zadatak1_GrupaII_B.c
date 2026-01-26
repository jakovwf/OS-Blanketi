/*
Zadatak 1. Koriscenjem programskog jezika C kreirati Linux program koji koristi
dve dodatne niti kopira sadrzaj datoteke ciji je naziv zadat kao argument komandne
linije u dve datoteke "parni.txt" i "neparni.txt" tako da datoteka "parni.txt"
sadrzi samo parne linije iz ulazne datoteke, a datoteka "neparni.txt" sadrzi
samo neparne linije iz ulazne datoteke. Glavna nit otvara ulaznu datoteku,
cita njen sadrzaj liniju po liniju i upisuje u globalnu promenljivu odakle je
preuzimaju dodatne niti. Sinhronizaciju rada niti resiti koriscenjem semafora.
(25 poena)
*/


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t parni, neparni, povratni;
char buf[1024];
int Gotovo =0;

void* NitParna(void* ){
    FILE* fp = fopen("parni.txt", "w");
    if(!fp) return NULL;

    while(1){
        sem_wait(&parni);
        if(Gotovo)  break;

        fputs(buf, fp);
        fflush(fp);
        sem_post(&povratni);
    }
    fclose(fp);
}
void* NitNeparna(void* ){
    FILE* fp = fopen("neparni.txt", "w");
    if(!fp) return NULL;

    while(1){
        sem_wait(&neparni);
        if(Gotovo)  break;

        fputs(buf, fp);
        fflush(fp);
        sem_post(&povratni);
    }
    fclose(fp);
}


int main(int argc, char* argv[]){
    if(argc!=2) return -1;



    sem_init(&parni, 0, 0);
    sem_init(&neparni, 0, 0);
    sem_init(&povratni, 0, 0);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, NitParna, NULL);
    pthread_create(&t2, NULL, NitNeparna, NULL);
    
    
    FILE* f=fopen(argv[1], "r");
    if(!f) return -1;
    int i=0;
    while(fgets(buf, sizeof(buf), f)){
        if(i%2==0){
            sem_post(&parni);
        }
        else{
            sem_post(&neparni);
        }

        sem_wait(&povratni);
        i++;
    }

    fclose(f);
    Gotovo=1;
    sem_post(&parni);
    sem_post(&neparni);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);


    sem_destroy(&parni);
    sem_destroy(&neparni);
    sem_destroy(&povratni);

}
