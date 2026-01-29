#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern int pd1[2], pd2[2];
extern char buf[1024];

void proces1()
{
    close(pd1[0]);
    close(pd2[1]);

    fgets(buf, 1024, stdin);

    if (buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = '\0';

    char rec[128];
    int x, len = 0;

    while (sscanf(buf + len, "%s", rec) == 1)
    {
        write(pd1[1], rec, 128);
        read(pd2[0], &x, sizeof(int));

        printf("Suma cifara u reci %s je %d\n", rec, x);
        len += strlen(rec) + 1;
    }

    close(pd1[1]);
    close(pd2[0]);
    wait(NULL);
}
