#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>


//Skroz optimizovan kod i radi sa svim brojevima, poenta strukture je da prosledi %n iz sscanf - to je broj brojeva koji je procitao ovaj sscanf
//kada se prosledi onda ne mora da for petlja u niti ide do LEN = 256 nego ide tacno onoliko koliko je potrebno
//sleepovi i fflush su iskljucivo tu zbog estetike i razumevanja i ne uticu na tacnost zadatka
//mora se inkrementirati brojac p u sscanf

#define LEN 256

sem_t citaj, sumiraj;
pthread_t nit1;
int suma = 0;
int kraj = 0; // flag za završetak

typedef struct {
    int niz[LEN];
    int count;
} Podaci;

void* prvaNit(void* arg) {
    Podaci* pod = (Podaci*)arg;

    while (!kraj) {            // dok flag nije postavljen
        sem_wait(&sumiraj);

        if (kraj) break;       // provera nakon čekanja

        suma = 0;
        printf("[Nit] Sabiram: ");
        for (int i = 0; i < pod->count; i++) {
            suma += pod->niz[i];
            printf("%d ", pod->niz[i]);
            fflush(stdout);
            usleep(300000);
        }
        printf("\n[Nit] Suma izračunata: %d\n", suma);

        sem_post(&citaj);
        usleep(200000);
    }

    printf("[Nit] Zavrsavam nit.\n");
    return NULL;
}

int main() {
    Podaci pod;
    memset(&pod, 0, sizeof(Podaci));

    char linija[LEN];

    FILE* ulaz = fopen("ulaz.txt", "r");
    if (!ulaz) {
        perror("Greska");
        exit(1);
    }

    sem_init(&citaj, 0, 0);
    sem_init(&sumiraj, 0, 0);

    pthread_create(&nit1, NULL, prvaNit, &pod);

    while (fgets(linija, LEN, ulaz)) {
        memset(&pod, 0, sizeof(Podaci));

        int i = 0;
        char* p = linija;
        int broj;
        int n;

        while (sscanf(p, "%d%n", &broj, &n) == 1 && i < LEN) {
            pod.niz[i++] = broj;
            p += n;
        }
        pod.count = i;

        sem_post(&sumiraj);  // budi nit
        sem_wait(&citaj);    // čekaj da završi

        printf("[Main] Rezultat sume: %d\n\n", suma);
        fflush(stdout);
        usleep(500000);
    }

    // Signal da nit završi
    kraj = 1;
    sem_post(&sumiraj);  // obavezno probuditi nit koja čeka
    pthread_join(nit1, NULL);

    sem_destroy(&citaj);
    sem_destroy(&sumiraj);
    fclose(ulaz);

    return 0;
}