#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/wait.h>
#include<math.h>
#include<string.h>


int isPrime(int n) {
    if (n <= 1)
        return 0;   // nije prost

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;   // nije prost
    }
    return 1;   // prost je
}
int main(int argc, char* argv[]){
    int pd[2];      // pipe roditelj -> dijete
    int pd2[2];     // pipe dijete -> roditelj
    int pid;
    char buffer[5];
    int broj;

    if(pipe(pd)<0){
        exit(1);
    }
    
    if(pipe(pd2)<0){
        exit(1);
    }

    pid=fork();

    if(pid!=0){
        // RODITELJ
        int brojProstih;
        close(pd[0]);       // roditelj ne čita iz pipe-a 1
        close(pd2[1]);      // roditelj ne piše u pipe 2

        while(1){
            printf("Unesi broj (ili 'Kraj' za izlaz): ");
            scanf("%s", buffer);
            
            if(strcmp(buffer, "Kraj") == 0){
                write(pd[1], "Kraj", strlen("Kraj")+1);
                break;
            }
            
            write(pd[1], buffer, strlen(buffer)+1);
        }
        
        printf("Unesen je Kraj\n");
        read(pd2[0], &brojProstih, sizeof(int));
        printf("Broj prostih brojeva: %d\n", brojProstih);
        
        close(pd[1]);
        close(pd2[0]);
        wait(NULL);
       
    }else{
        // DIJETE
        int prost=0;
        int brojevi;
        close(pd[1]);       // dijete ne piše u pipe 1
        close(pd2[0]);      // dijete ne čita iz pipe 2
        
        while(1){
            read(pd[0], buffer, sizeof(buffer));
            
            if(strcmp(buffer, "Kraj") == 0){
                break;
            }
            
            brojevi = atoi(buffer);
            if(isPrime(brojevi)){
                prost++;
            }
        }
        
        close(pd[0]);
        write(pd2[1], &prost, sizeof(int));
        close(pd2[1]);
        exit(0);
    }
    return 0;
}
