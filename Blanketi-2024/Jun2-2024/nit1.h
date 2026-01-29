#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

extern sem_t prvi, drugi;
extern int i, N;

void *prvaStampa(void *arg)
{
    while (i <= N)
    {
        sem_wait(&prvi);

        if (i <= N && i % 7 == 0)
        {
            sleep(1);
            printf("Deljiv sa 7: %d\n", i++);
        }

        sem_post(&drugi);
    }
    return NULL;
}
