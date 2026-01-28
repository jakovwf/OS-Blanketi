#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int broj;
int kraj = 0;
int stampanje = 0;

pthread_cond_t stampaj;
pthread_cond_t gotovo;
pthread_mutex_t m;

void* StampajOstale(void* a)
{
    while (1) {
        pthread_mutex_lock(&m);

        while (stampanje == 0 && kraj == 0)
            pthread_cond_wait(&stampaj, &m);

        if (kraj == 1) {
            pthread_mutex_unlock(&m);
            return NULL;
        }
        if(broj % 7 != 0){
            printf("%d\n", broj);
            stampanje = 0;
            pthread_cond_signal(&gotovo);
        }
        pthread_mutex_unlock(&m);
    }
}

void* StampajSa7(void* a)
{
    while (1) {
        pthread_mutex_lock(&m);

        while (stampanje == 0 && kraj == 0)
            pthread_cond_wait(&stampaj, &m);

        if (kraj == 1) {
            pthread_mutex_unlock(&m);
            return NULL;
        }
        if(broj % 7 == 0){
            printf("%d\n", broj);
            stampanje = 0;
            pthread_cond_signal(&gotovo);
        }
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
        exit(1);

    int N = atoi(argv[1]);

    pthread_t t1, t2;

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&stampaj, NULL);
    pthread_cond_init(&gotovo, NULL);

    pthread_create(&t1, NULL, StampajSa7, NULL);
    pthread_create(&t2, NULL, StampajOstale, NULL);

    for (int i = 0; i <= N; i++) {
        pthread_mutex_lock(&m);

        broj = i;
        stampanje = 1;

        pthread_cond_broadcast(&stampaj);

        while (stampanje == 1)
            pthread_cond_wait(&gotovo, &m);

        pthread_mutex_unlock(&m);
    }

    pthread_mutex_lock(&m);
    kraj = 1;
    pthread_cond_broadcast(&stampaj);
    pthread_mutex_unlock(&m);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_cond_destroy(&stampaj);
    pthread_cond_destroy(&gotovo);
    pthread_mutex_destroy(&m);

    return 0;
}
