#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>

pthread_t nit1;
pthread_mutex_t mut1;
pthread_cond_t cond1;
int br,spreman=0;

void*prvaNIt(void*arg){
    while(1){
        
        pthread_mutex_lock(&mut1);
       
        while(!spreman){
            pthread_cond_wait(&cond1,&mut1);
        }
         if(br==0){
            pthread_mutex_unlock(&mut1);
            break;
        }
        printf("Pocinjem da brojim iz druge niti:\n");
        for(int i=br;i>=0;i--){
            printf("%d\n",i);
            sleep(1);   //originalno je 4 u zadatku ali me mrzi da cekam tako da...
        }
        
        spreman=0;
        pthread_mutex_unlock(&mut1);
        pthread_cond_signal(&cond1);

    }
}

int main(int argc,char*argv[]){
    
    pthread_create(&nit1,NULL,prvaNIt,NULL);
    pthread_mutex_init(&mut1,NULL);
    pthread_cond_init(&cond1,NULL);


    while(1){
        pthread_mutex_lock(&mut1);
        while(spreman){
            pthread_cond_wait(&cond1,&mut1);
        }
        printf("Javljam se iz main niti\n");
        printf("Unesi broj: ");
        scanf("%d",&br);
       
        spreman=1;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&mut1);
        sleep(1); //nema potrebe ali cisto da se vidi sve lepo
         if(br==0){
            pthread_mutex_unlock(&mut1);
            break;
        }
    }


    pthread_join(nit1,NULL);

    pthread_mutex_destroy(&mut1);
    pthread_cond_destroy(&cond1);
    return 0;
}