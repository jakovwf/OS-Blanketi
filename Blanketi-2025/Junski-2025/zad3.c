#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<string.h>

#define KEY 10101
#define LEN 256

struct msg 
{
    long tip;
    int indeks;
    char ime[LEN];
    char prezime[LEN];
};
int main(int argc, char* argv[]){

    struct msg buffer;

    int msqid=msgget(KEY, 0666| IPC_CREAT);

    int pid=fork();
    int pid2=fork();
    if(pid!=0 && pid2!=0){
        do{
        printf("Unesite broj indeksa, ime i prezime:\n");
        scanf("%d %s %s", &buffer.indeks,buffer.ime, buffer.prezime);
        if(buffer.indeks<17000){
            buffer.tip=1;
        }else{
            buffer.tip=2;
        }
        msgsnd(msqid, &buffer, sizeof(buffer)-sizeof(long), 0);

        }while(buffer.indeks!=10000);
        if(buffer.indeks==10000){
            buffer.tip=1;
            msgsnd(msqid, &buffer, sizeof(buffer)-sizeof(long), 0);
            buffer.tip=2;
            msgsnd(msqid, &buffer, sizeof(buffer)-sizeof(long), 0);
        }
        wait(NULL);
        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
    }else if(pid==0){
        FILE*f=fopen("stara_akreditacija.txt", "w");
        int brojac=0;
        while(1){
            msgrcv(msqid, &buffer, sizeof(buffer)-sizeof(long), 1, 0);
            if(buffer.indeks==10000){
                break;
            }
            fprintf(f, "Broj indeksa:%d\n Ime:%s\n Prezime:%s\n", buffer.indeks, buffer.ime, buffer.prezime);
            brojac++;
        }
        fclose(f);
        printf("Stara akreditacija:%d studenata\n", brojac);
    }
    else if(pid2==0){
        FILE* f=fopen("nova_akreditacija.txt", "w");
        int brojac=0;
        while(1){
            msgrcv(msqid, &buffer, sizeof(buffer)-sizeof(long), 2, 0);
            if(buffer.indeks==10000){
                break;
            }
            fprintf(f, "Broj indeksa:%d\n Ime:%s\n Prezime:%s\n", buffer.indeks, buffer.ime, buffer.prezime);
            brojac++;
        }
        fclose(f);
        printf("Nova akreditacija:%d studenata\n", brojac);
    }
    
    return 0;
}
