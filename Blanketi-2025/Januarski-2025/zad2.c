#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>


//PRIMERI ZA TEST - ./zad2 /bin/code zad5.c - OTVARA SE CODE I PRAVI ZAD5.C
//./zad2 /bin/ls -l 

int main(int argc,char*argv[]){
    
    if(argc<3){
        printf("Nema dovoljno argumenata!!!");
        exit(1);
    }

    int pid;
    char prviArg[strlen(argv[1])+1],drugiArg[strlen(argv[2])+1];
    strcpy(prviArg,argv[1]);
    strcpy(drugiArg,argv[2]);

    pid = fork();
    if(pid<0){
        perror("Los proces");
        exit(1);
    }
    if(pid==0){
        //dete
        execl(prviArg,prviArg,drugiArg,NULL);
        perror("Execl los");
        exit(1);
    }else{
        //roditelj
        int status;
        waitpid(pid,&status,0); //ceka dete da zavrsi
        printf("Status: %d\n",status);
    }    
    return 0;
}