/*
Zadatak 1. Koriscenjem programskog jezika C kreirati Linux program koji
kreira dve dodatne niti, a zatim u glavnoj niti ucitava od korisnika recenicu.
Druga nit u ovoj recenici menja pocetno slovo u veliko i dodaje tacku na kraj
recenice ukoliko ona nedostaje. Treca nit u recenici menja blanko znake
podvlakom "_" i zatim stampa tako izmenjenu recenicu. Postupak se ponavlja
sve dok korisnik ne unese rec KRAJ. Poznatim mehanizmima za sinhronizaciju
niti obezbediti da se niti izvrsavaju naizmenicno. (25 poena)
*/

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>

sem_t prvi, drugi, gotovo;
int Kraj =0;
char buf[1024];

void* SlovoITacka(void*){
    
    while(1){
        sem_wait(&prvi);
        if(Kraj){
            sem_post(&drugi);
            return NULL;
        }

        if(buf[0]>='a' && buf[0]<='z')
            buf[0]-= 'a'-'A';


        int duz = strlen(buf);
        if(buf[duz-1]!='.'){
            buf[duz] = '.';
            buf[duz+1] = '\0';
        }

        sem_post(&drugi);
    }
    
}

void* BlankoZnak(void*){
    
    while(1){
        sem_wait(&drugi);
        if(Kraj)
            return NULL;


        for(int i=0;i<strlen(buf);i++){
            if(buf[i]==' ') 
                buf[i]='_';
        }

        fputs(buf, stdout);
        fflush(stdout);

        sem_post(&gotovo);
    }
    
}

int main(){
    sem_init(&prvi, 0, 0);
    sem_init(&drugi, 0, 0);
    sem_init(&gotovo, 0, 0);

    pthread_t t1, t2;
    pthread_create(&t1, 0, SlovoITacka, NULL);
    pthread_create(&t2, 0, BlankoZnak, NULL);

    while(1){
        printf("\nUnesi recenicu: ");
        fflush(stdout);

        fflush(stdin);
        fgets(buf,sizeof(buf), stdin);

        if(buf[strlen(buf)-1]=='\n')
            buf[strlen(buf)-1] = '\0';

        if(!strcmp(buf, "KRAJ")) break;

        sem_post(&prvi);
        sem_wait(&gotovo);
    }

    Kraj=1;
    sem_post(&prvi);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&prvi);
    sem_destroy(&drugi);
    sem_destroy(&gotovo);


    return 0;

}


