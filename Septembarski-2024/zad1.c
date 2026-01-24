
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//DRUGA JE NAZIV ZATO STO NISAM LEPO PROCITAO ZADATAK, TREBA SAMO JEDNA NIT DA SE NAPRAVI A NE
//DVE, SAMO MAIN NIT I JEDNA DODATNA...


pthread_t prvaNit, drugaNit;
pthread_mutex_t mut;
pthread_cond_t cond;

int br = 0;
int spremno = 0;



void* druga(void* atr) {
    while (1) {
        pthread_mutex_lock(&mut);

        while (!spremno)
            pthread_cond_wait(&cond, &mut);

        if (br == 999) {
            pthread_mutex_unlock(&mut);
            break;
        }

        printf("Druga nit krece:\n");
        for (int i = 0; i < br; i++) {
            printf("%d\n", i);
            sleep(1);
        }

        spremno = 0;
        pthread_cond_signal(&cond); // javi prvoj niti da je gotovo

        pthread_mutex_unlock(&mut);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);


    pthread_create(&drugaNit, NULL, druga, NULL);

    while (1) {
        pthread_mutex_lock(&mut);

        // cekaj dok druga nit ne zavrsi
        while (spremno)
            pthread_cond_wait(&cond, &mut);

        printf("\nUnesi broj (999 za izlaz): ");
        scanf("%d", &br);

        spremno = 1;
        pthread_cond_signal(&cond);

        if (br == 999) {
            pthread_mutex_unlock(&mut);
            break;
        }

        pthread_mutex_unlock(&mut);
    }
    return NULL;

    pthread_join(drugaNit, NULL);

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
    return 0;
}