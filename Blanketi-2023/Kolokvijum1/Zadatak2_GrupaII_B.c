
/*
Zadatak 2. Koriscenjem programskog jezika C kreirati Linux program koji kreira 
dodatni process sa kojim komunicira koriscenjem datavoda. Prvi process otvara 
tekstualnu datoteku ciji se naziv zadaje kao argument komandne linije, cita liniju 
po liniju i koriscenjem datavoda drugom procesu salje samo linije koje su duzine 
preko 80 karaktera tako sto salje redni broj linije i sadrzaj linije. Drugi process 
na ekranu treba da odstampa samo parne linije koje je dobio. (25 poena)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char* argv[]){
    if(argc !=2) 
        exit(1);
    
    int pd[2];
    if(pipe(pd) <0)
        exit(1);

    int pid = fork();
    if(pid<0)
        exit(1);
    
    if(pid!=0){
        //prvi
        close(pd[0]);

        FILE* f = fopen(argv[1], "r");
        if(!f) 
            exit(1);
        char buf[1024];
        int i=0;
        while(fgets(buf, 1024, f) != NULL){
            
            int len = strlen(buf);
            if(buf[len-1] =='\n')
                buf[len-1] = '\0';

            if(len>80){
                write(pd[1], &len, sizeof(int));
                write(pd[1], buf, sizeof(char)*len);
                
            }
            i++;
        }
        fclose(f);
        close(pd[1]);
        wait(NULL);
        return 0;

    }
    else{
        close(pd[1]);

        char buf[1024];
        int len;
        while(read(pd[0], &len, sizeof(int))){

            read(pd[0], buf, sizeof(char)*len);
            if(len%2==0)
                printf("%s\n", buf);
        }
        close(pd[0]);
        exit(0);
    }

}