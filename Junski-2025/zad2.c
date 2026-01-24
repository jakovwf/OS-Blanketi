#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

//OBJASNJENJE//
//POTREBAN JE JEDAN DATAVOD DA BI DVA PROCESA KOMUNICIRALA
//NA POCETKU PRAVIM DATAVOD I PROVERAVAM DA LI JE ISPRAVNO NAPRAVLJEN
//POSLE TOGA PRAVIM PROCES
//PRVO KUCAM KOD ZA RODITELJA ODNOSNO PID!=0, ZATVARAM STRANU DATAVODA
//KOJA JE ZA CITANJE SA DATAVODA ODNOSNO PD[0], U WHILE KRECEM DA UPISUJEM 
//BROJ PO BROJ, PROCES DETE ZATVARA P[1] ODNOSNO STRANU DATAVODA ZA UPIS
//S OBZIROM NA TO DA READ VRACA INT CITACE SVE DOK NE OCITA -1 ODNOSNO KRAJ
//ISPITUJEM DA LI JE BROJ PARAN I AKO JESTE PRINTUJEM , AKO NIJE INKREMENTIRAM 
//BROJAC ZA NEPARAN BROJ A POSLE TOGA ISPITUJEM DA LI JE BROJ NEPARNIH 3
//AKO JESTE POZIVAM BREAK A AKO NIJE NASTAVLJA SE DALJE.


int main(int argc,char*argv[]){
    srand(time(NULL));  //seed da bi se na svako pokretanje bili drugi brojevi

    int pd[2]; //p[0] i p[1] za write i read
    int pid;  
    int broj,broj1;
    int brNep=0;
    

    if(pipe(pd)<0){
        printf("Neuspesno kreiran datavod");
        exit(1);
    }
    pid = fork();
    if(pid==0){
        //dete
        close(pd[1]);    
        while((read(pd[0],&broj1,sizeof(int)))>0){
            printf("Dete: %d\n",broj1);
            if((broj1%2)==0){
                printf("Parni!!!\n\n");        
            }else{
                brNep++;
                printf("Neparni!!!\n\n");
            }
            if(brNep==3){
                printf("Kraj!!! - 3 NEPARNA\n\n");
                break;
            }

        }
        close(pd[0]);
    }
    else{
        //roditelj
        close(pd[0]);
        while(1){
            int rndm = rand()%99; //rndm broj izmedju 0 i 99
            broj=rndm;
            write(pd[1],&broj,sizeof(int));
            //sleep(1)  //nije obavezno ali pokazuje u real-time-u kako se brojevi prenose preko datavoda
        }
        close(pd[1]);
    }

    return 0;
}