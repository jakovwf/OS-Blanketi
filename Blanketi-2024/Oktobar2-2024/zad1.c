#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

#define LEN 256

//bitno je koji se fajl navodi prvi zato sto mora ili da ima jednako linija ili jedna vise za prvi fajl koji se cita
//zato sto cim bude izaslo iz while nece uopste nista ispisati nista i doci ce do toga da fali jedan red...
//ovo moze da se resi time sto se osigura da program kada izadje ispise makar nesto i da ne prebacuje kraj na 1
//ali gubljenje je vremena i mislim da je nepotrebno ici toliko u dubinu...
//TEST - ./zad1  fajl1.txt  fajl.txt
//TEST KADA NECE RADITI SKROZ PRAVILNO - ./zad1  fajl.txt  fajl1.txt

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //umesto ovoga ide pthread_mutex_initi(...)
pthread_cond_t cond_parni = PTHREAD_COND_INITIALIZER;//umesto ovoga ide pthread_cond_init(...)
pthread_cond_t cond_neparni = PTHREAD_COND_INITIALIZER;
pthread_t nit1,nit2;
int trenutna_nit = 1; // 1 = prva nit (parni), 2 = druga nit (neparni)
int kraj = 0;

void* citajPrviFajl(void* arg){
    char* imeFajla = (char*)arg;

    FILE *f = fopen(imeFajla, "r");
    if(f == NULL){
        printf("Ne mogu da otvorim fajl: %s\n", imeFajla);
        exit(1);
    }
    
    char linija[LEN];
    int brlinije = 0;
    
    while(fgets(linija, LEN, f) != NULL){
        pthread_mutex_lock(&mutex);
        
        // Čekamo dok ne dođe red na parnu liniju
        while(trenutna_nit != 1 && !kraj){
            pthread_cond_wait(&cond_parni, &mutex);
        }
        
        if(kraj){
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        // Ispis u konzolu
        printf("%s", linija);
        
        // Upis u fajl (parne linije)
        FILE *f1 = fopen("zbir.txt", "a");
        fprintf(f1, "%s(linija %d):%s", imeFajla, brlinije, linija);
        fclose(f1);
        
        sleep(1);
        
        // Sada je red na drugu nit (neparne linije)
        trenutna_nit = 2;
        brlinije++;
        
        // Budimo drugu nit
        pthread_cond_signal(&cond_neparni);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_lock(&mutex);
    kraj = 1;
    pthread_cond_signal(&cond_neparni); // Budimo drugu nit ako čeka
    pthread_mutex_unlock(&mutex);
    
    fclose(f);
    return NULL;
}

void* citajDrugiFajl(void* arg){
    char* imeFajla = (char*)arg;

    FILE *f = fopen(imeFajla, "r");
    if(f == NULL){
        printf("Ne mogu da otvorim fajl: %s\n", imeFajla);
        exit(1);
    }
    
    char linija[LEN];
    int brlinije = 0;
    
    while(fgets(linija, LEN, f) != NULL){
        pthread_mutex_lock(&mutex);
        
        // Čekamo dok ne dođe red na neparnu liniju
        while(trenutna_nit != 2 && !kraj){
            pthread_cond_wait(&cond_neparni, &mutex);
        }
        
        if(kraj){
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        // Ispis u konzolu
        printf("%s", linija);
        
        // Upis u fajl (neparne linije)
        FILE *f1 = fopen("zbir.txt", "a");
        fprintf(f1, "%s (linija %d): %s", imeFajla, brlinije, linija);
        fclose(f1);
        
        sleep(1);
        
        // Sada je red na prvu nit (parne linije)
        trenutna_nit = 1;
        brlinije++;
        
        // Budimo prvu nit
        pthread_cond_signal(&cond_parni);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_lock(&mutex);
    kraj = 1;
    pthread_cond_signal(&cond_parni); // Budimo prvu nit ako čeka
    pthread_mutex_unlock(&mutex);
    
    fclose(f);
    return NULL;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Neodgovarajuci broj argumenata\n");
        printf("Upotreba: %s fajl1.txt fajl2.txt\n", argv[0]);
        exit(1);
    }

    // Obriši postojeći zbir.txt
    remove("zbir.txt");
    
    // Kreiraj niti
    // Prva nit (parni) čita PRVI fajl (argv[1])
    // Druga nit (neparni) čita DRUGI fajl (argv[2])
    pthread_create(&nit1, NULL, citajPrviFajl, argv[1]);
    pthread_create(&nit2, NULL, citajDrugiFajl, argv[2]);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    // Uništi mutex i uslovne promenljive
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_parni);
    pthread_cond_destroy(&cond_neparni);

    return 0;
}