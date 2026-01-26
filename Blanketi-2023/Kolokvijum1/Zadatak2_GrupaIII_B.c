/*
Zadatak 2. Koriscenjem programskog jezika C kreirati 
Linux program koji kreira dodatni proces dete sa kojim 
komunicira preko datavoda. Proces roditelj od korisnika 
ucitava rec i salje je procesu detetu. Proces dete proverava 
da li se dobijena rec nalazi u spisku reci koji se nalaze 
u fajlu reci.txt i u skladu sa tim odgovara procesu roditelju 
preko datavoda sa FOUND, ukoliko je nasao rec, ili sa NOT FOUND 
ukoliko nije nasao rec. Proces roditelj stampa dobijeni odgovor. 
Postupak se ponavlja sve dok korisnik ne unese rec END. (25 poena)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 128
int main(){
    int pd1[2], pd2[2];
    if(pipe(pd1) < 0) exit(1);
    if(pipe(pd2) < 0) exit(1);

    int pid = fork();
    if(pid!=0){
        close(pd1[0]);
        close(pd2[1]);

        char buf[MAX];
        char odg[10];
        while(1){
            scanf("%s", buf);
            write(pd1[1], buf, sizeof(char)* MAX);

            if(!strcmp(buf, "END")) 
                break;

            read(pd2[0], odg, 10);
            printf("%s\n\n", odg);
        }


        close(pd1[1]);
        close(pd2[0]);
        wait(NULL);
        return 0;
    }
    else{
        close(pd1[1]);
        close(pd2[0]);

        char buf[MAX];

        while(1){
            
            read(pd1[0], buf, sizeof(char)* MAX);

            if(!strcmp(buf, "END")) 
                break;


            FILE* f = fopen("reci.txt", "r");
            if(!f) 
                exit(1);

            int nadjen =0;
            char pom[MAX];

            while((fgets(pom, MAX, f))!=0){
                
                if(pom[strlen(pom)-1] == '\n')
                    pom[strlen(pom)-1] = '\0';

                if(!strcmp(pom, buf)){
                    nadjen =1;
                    break;
                }
            }
            if(nadjen){
                write(pd2[1], "FOUND", 6);
            }
            else{
                write(pd2[1], "NOT FOUND", 9);
            }

            fclose(f);
        }

        close(pd1[0]);
        close(pd2[1]);
        exit(0);
    }
}