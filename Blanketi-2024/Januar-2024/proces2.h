#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern int pd1[2], pd2[2];

void proces2()
{
    close(pd1[1]);
    close(pd2[0]);

    char rec[128];
    int suma;

    while (read(pd1[0], rec, 128) > 0)
    {
        suma = 0;
        for (int i = 0; i < strlen(rec); i++)
            if (rec[i] >= '0' && rec[i] <= '9')
                suma += rec[i] - '0';

        write(pd2[1], &suma, sizeof(int));
    }

    close(pd1[0]);
    close(pd2[1]);
    exit(0);
}
