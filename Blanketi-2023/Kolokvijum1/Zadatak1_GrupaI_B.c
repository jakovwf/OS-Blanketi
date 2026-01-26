#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t m;
pthread_cond_t obavesti;

int N = 0;
int Stampaj = 0;
int Gotovo = 0;

void* StampajIzFajla(void* arg)
{
    while (1) {
        pthread_mutex_lock(&m);

        while (!Stampaj && !Gotovo)
            pthread_cond_wait(&obavesti, &m);

        if (Gotovo) {
            pthread_mutex_unlock(&m);
            return NULL;
        }

        int pom = N;
        pthread_mutex_unlock(&m);

        FILE* f = fopen("data.txt", "r");
        if (!f) return NULL;

        char linija[1024];
        for (int i = 0; i < pom; i++) {
            if (!fgets(linija, sizeof(linija), f)) break;
            fputs(linija, stdout);
            fflush(stdout);
            sleep(1);
        }
        fclose(f);

        pthread_mutex_lock(&m);
        Stampaj = 0;
        pthread_cond_signal(&obavesti);
        pthread_mutex_unlock(&m);
    }
}

int main(void)
{
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&obavesti, NULL);

    pthread_t nit;
    pthread_create(&nit, NULL, StampajIzFajla, NULL);

    while (1) {
        int x;
        printf("Unesi N: ");
        fflush(stdout);
        if (scanf("%d", &x) != 1) break;

        pthread_mutex_lock(&m);

        if (x == 99) {
            Gotovo = 1;
            pthread_cond_signal(&obavesti);
            pthread_mutex_unlock(&m);
            break;
        }

        while (Stampaj)
            pthread_cond_wait(&obavesti, &m);

        N = x;
        Stampaj = 1;
        pthread_cond_signal(&obavesti);

        while (Stampaj)
            pthread_cond_wait(&obavesti, &m);

        pthread_mutex_unlock(&m);
    }

    pthread_join(nit, NULL);
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&obavesti);
    return 0;
}
