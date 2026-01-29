#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

extern sem_t prvi, drugi;
extern int i, N;

void *drugaStampa(void *arg)
{
    while (i <= N)
    {
        sem_wait(&drugi);

        while (i <= N && i % 7 != 0)
        {
            sleep(1);
            printf("Broj: %d\n", i++);
        }

        sem_post(&prvi);
    }
    return NULL;
}
