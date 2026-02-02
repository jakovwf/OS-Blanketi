#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/wait.h>

#define KEY 10101
#define PONAVLJANJA 10

struct msg{
    long tip;
    int broj;
};

// funkcija za sabiranje cifara trocifrenog broja
int saberi(int broj){
    int suma = 0;
    while(broj > 0){
        suma += broj % 10;
        broj /= 10;
    }
    return suma;
}

int main(int argc, char* argv[]){
    struct msg buffer;
    buffer.tip=4;
    int msqid=msgget(KEY, 0666|IPC_CREAT);
    int pid=fork();
    if(pid!=0){
        for(int i=0;i<PONAVLJANJA;i++){
            scanf("%d", &buffer.broj);
            msgsnd(msqid, &buffer, sizeof(buffer)-sizeof(long), 0);
            }
        wait(NULL);
    }else{
        for(int i=0;i<PONAVLJANJA;i++){
            msgrcv(msqid, &buffer,sizeof(buffer)-sizeof(long), 4, 0);
                int suma = saberi(buffer.broj);
                printf("Ponavljanje:%d\nBroj:%d\nSuma cifara:%d\n", i+1, buffer.broj, suma);
        }
    }

    return 0;
}