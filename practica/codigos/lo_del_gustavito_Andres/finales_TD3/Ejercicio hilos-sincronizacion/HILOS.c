#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h>
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"

char buff[256],t=0;
pthread_t hilo[2];
int rc,arre[2];


void *hiloA(void *a){
strncpy(buff,MENSAJE1,sizeof(MENSAJE1));

printf("mensaje copiado\n");
pthread_exit(NULL);
}
void *hiloB(void *b){
strncpy(buff,MENSAJE2,sizeof(MENSAJE1));
printf("mensaje copiado\n");
pthread_exit(NULL);
}

int main() {


printf("Main creando el hilo nro %d\n", t);
     arre[t] = t;
     
     //rc = pthread_create(&hilo[t], NULL, hola , (void *)(&t)  );
     rc = pthread_create(&hilo[t], NULL, hiloA , (void *) (&arre[t]) );
     
     if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);   }     

pthread_join(hilo[t],NULL);
printf(" El mensaje recibido es %s y fue enviado por el hilo[%d]A\n",buff,t);
t++;

printf("Main creando el hilo nro %d\n", t);
     arre[t] = t;
     rc = pthread_create(&hilo[t], NULL, hiloB , (void *) (&arre[t]) );
     
     if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);        }

pthread_join(hilo[t],NULL);

printf(" El mensaje recibido es %s y fue enviado por el hilo[%d]B\n",buff,t);

sleep(2);
exit(0);
}


