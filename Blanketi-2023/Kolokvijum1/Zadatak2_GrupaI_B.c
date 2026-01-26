/*
Zadatak 2. Koriscenjem programskog jezika C kreirati Linux program 
koji kreira dva dodatna procesa koji su datavodima povezani u red. 
Prvi process generise slucajni broj u opsegu od 0 do 99 i putem 
datavoda ga salje drugom procesu. Drugi process ukoliko je primljeni 
broj paran deli ga sa dva (ukoliko je primljeni broj neparan ne modifikuje ga) 
i salje koriscenjem datavoda trecem procesu. Treci process odstampa primljeni 
broj ukoliko je veci od 10, ukoliko nije ne stampa nista. Standardni izlaz 
redirektovati na fajl "redirekcija.txt". Process se prekida posle 10 
generisanih i obradjenih brojeva. (25 poena)
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(){
    int pd1[2], pd2[2];
    if(pipe(pd1)<0)
        exit(1);
    if(pipe(pd2)<0)
        exit(1);

    int pid1, pid2;
    pid1=fork();
    if(pid1 < 0) 
    exit(1);

    

    if( pid1 != 0){

        pid2=fork();
        if(pid2 < 0) 
            exit(1);
        
        if(pid2!=0){
            //prvi

            srand(time(NULL));
            close(pd1[0]);
            close(pd2[0]);
            close(pd2[1]);

            for(int i=0;i<10;i++){
                int num = rand() %100;
                write(pd1[1], &num, sizeof(int));
            }
            close(pd1[1]);

            wait(NULL);
            wait(NULL);
        }
        else{
            //treci
            close(pd2[1]);
            close(pd1[0]);
            close(pd1[1]);
            /*
            Redirekcija moze  i ovako:
            FILE* fd = fopen("izlaz.txt", "w");
            int f = fileno(fd);
            dup2(f, STDOUT_FILENO);
            ili:
            */
            int f = open("redirekcija.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
            dup2(f, STDOUT_FILENO);
            close(f);

            int num;
            while((read(pd2[0], &num, sizeof(int))) >0){
                if(num>10){
                    printf("%d\n", num);
                }
            }
            close(pd2[0]);

            exit(0);
        }
    }
    else {
        //drugi
        close(pd1[1]);
        close(pd2[0]);
        for(int i=0;i<10;i++){
            int num;
            read(pd1[0], &num, sizeof(int));
            num%2==0 ? num/=2 : num;
            write(pd2[1], &num, sizeof(int)); 
        }
        close(pd1[0]);
        close(pd2[1]);
        exit(0);
    }


    return 0;
}
