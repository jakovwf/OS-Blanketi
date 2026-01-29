#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

#define LEN 256

sem_t sortira, stampa;
char **recenica1;
char recenica[LEN];
char temp[LEN];
int N;

void* Sortiraj(void* arg) {
    sem_wait(&sortira);

    strcpy(temp, recenica);

    int i = 0;
    char *rec = strtok(temp, " \n");
    while (rec != NULL) {
        i++;
        rec = strtok(NULL, " \n");
    }

    N = i;
    recenica1 = malloc(N * sizeof(char*));

    rec = strtok(recenica, " \n");
    int j = 0;
    while (rec != NULL) {
        recenica1[j] = malloc(strlen(rec) + 1);
        strcpy(recenica1[j], rec);
        j++;
        rec = strtok(NULL, " \n");
    }

    char pom[LEN];
    for (int l = 0; l < N - 1; l++) {
        for (int m = l + 1; m < N; m++) {
            if (strcmp(recenica1[l], recenica1[m]) > 0) {
                strcpy(pom, recenica1[l]);
                strcpy(recenica1[l], recenica1[m]);
                strcpy(recenica1[m], pom);
            }
        }
    }

    sem_post(&stampa);
    return NULL;
}

void* Stampaj(void* arg) {
    sem_wait(&stampa);

    recenica1[0][0] = toupper((unsigned char)recenica1[0][0]);

    char finalRecenica[LEN];
    finalRecenica[0] = '\0';

    for (int i = 0; i < N; i++) {
        strcat(finalRecenica, recenica1[i]);
        if (i != N - 1)
            strcat(finalRecenica, " ");
    }

    strcat(finalRecenica, ".");
    printf("%s\n", finalRecenica);

    sem_post(&sortira);
    return NULL;
}

int main() {
    fgets(recenica, LEN, stdin);

    sem_init(&sortira, 0, 1);
    sem_init(&stampa, 0, 0);

    pthread_t nit1, nit2;
    pthread_create(&nit1, NULL, Sortiraj, NULL);
    pthread_create(&nit2, NULL, Stampaj, NULL);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    sem_destroy(&sortira);
    sem_destroy(&stampa);

    return 0;
}
