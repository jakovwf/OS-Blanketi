#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "proces1.h"
#include "proces2.h"

/* GLOBALNE promenljive */
int pd1[2], pd2[2];
char buf[1024];
int pid;

int main()
{
    if (pipe(pd1) < 0) exit(1);
    if (pipe(pd2) < 0) exit(1);

    pid = fork();

    if (pid > 0)
    {
        proces1();
    }
    else
    {
        proces2();
    }

    return 0;
}
