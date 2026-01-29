#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "proces1.h"
#include "proces2.h"

//ISTI PRINCIP KAO SA NITIMA, POSTO SE PROCESI TAKODJE IZVRSAVAJU PARALELNO ILI KVAZI PARALELNO
//MOZEMO IH SMESTITI U RAZLICITE .H FAJLOVE RADI BOLJE PREGLEDNOSTI I DA SAMO POZOVEMO FUNKCIJE ODNOSNO
//PROCES KOJI BI TREBALO DA SE IZVRSI, TAKO U MAINU IMAMO ODLICAN PREGLED KODA JER IMAMO SAMO USLOVE I POZIVE
// A U .H FAJLOVIMA IMAMO ISKLJUCIVO KOD PROCESA KOJI CEMO POZVATI


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
