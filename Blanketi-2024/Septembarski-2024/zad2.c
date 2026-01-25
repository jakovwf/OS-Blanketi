#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/types.h>
#include<string.h>

int main(int argc,char*argv[]){

    if(argc<3){
        printf("Nema dovoljno arg!");
        exit(1);
    }
    
    char dat1[strlen(argv[1])+1],dat2[strlen(argv[2])+1],buff1[1000],buff2[1000];
    strcpy(dat1,argv[1]);
    strcpy(dat2,argv[2]);

    FILE * f1,*f2;
    f1=fopen(dat1,"rb");
    f2=fopen(dat2,"wb");

    if(!f1||!f2){
        perror("Nije ucitan fajl");
        exit(1);
    }

    int pid,pd[2],n;

    if(pipe(pd)<0){
        perror("Los pajp");
        exit(1);
    }

    pid=fork();

    if(pid<0){
        perror("Los proces");
        exit(1);
    }

    if(pid==0){
        //dete
        close(pd[1]);
        while((n=read(pd[0],buff2,sizeof(buff2)))>0){
            
            fwrite(buff2,1,n,f2);
            printf("Procitno\n");
        }
        close(pd[0]);
    }
    else{
        //roditelj
        int brojBajta=0;
        close(pd[0]);
        
        while(fgets(buff1,1000,f1)!=NULL){
            int len =strlen(buff1);
            for(int i=0;i<len;i++){
                buff1[i]^=brojBajta++;
            }
            write(pd[1],buff1,strlen(buff1));
            printf("Procitano 1000\n");
        }
        close(pd[1]);
        wait(NULL);
    }
    
    fclose(f1);
    fclose(f2);

    return 0;
}