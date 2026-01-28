#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
int main()
{
    int pd1[2], pd2[2];

    if(pipe(pd1) <0){
        exit(1);
    }
    if(pipe(pd2) <0){
        exit(1);
    }


    char buf[1024];
    int pid = fork();

    if(pid != 0){
        //glvani


        close(pd1[0]);
        close(pd2[1]);

        fflush(stdin);

        fgets(buf, 1024, stdin);
        
        if(buf[strlen(buf)-1] =='\n')
            buf[strlen(buf)-1] = '\0';
        char rec[128];
        
        int x;
        int len =0;
        while(sscanf(buf+len, "%s", rec) > 0){
            write(pd1[1], rec, 128);
            read(pd2[0], &x, sizeof(int));
        
            printf("Suma cifara u reci %s je %d.\n", rec, x);
            len += strlen(rec)+1;
        }


        close(pd1[1]);
        close(pd2[0]);
        wait(NULL);
    }
    else{

        close(pd1[1]);
        close(pd2[0]);

        char rec[128];
        
        int suma;
       
        while(read(pd1[0], rec, 128) > 0){
            suma=0;
            for(int i=0;i< strlen(rec);i++){
                if(rec[i] >= '1' && rec[i] <= '9')
                    suma+= rec[i] - '0';
            }

            write(pd2[1], &suma, sizeof(int));
        }
        
        close(pd1[0]);
        close(pd2[1]);
        exit(1);
    }

    return 0;

}