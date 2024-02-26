/*
Tres procesos (A, B, C) se comunican por medio de una Cola de Mensaje.
El proceso A escribe en la Cola de Mensaje "0123456789" en forma atómica y termina.
El proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" en forma atómica y termina.
El proceso C espera a que los procesos A y B terminen, lee los mensajes de la cola de mensajes y los muestra.
*/

//Compile el programa: gcc -o final_muestra final_muestra.c -lpthread -lrt
//Ejecute el programa ./final_muestra


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

#define MENSAJE_1 "0123456789"
#define MENSAJE_2 "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3" 

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex
pthread_mutex_t mtx1=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex

 mqd_t descriptor_mq;
      struct mq_attr attr,attr_rcv;
pthread_t hilo[5];

int rc, er;
int arre[5];
int leido;
char buff[80],buff1[80], buff2[80];
pthread_t th1, th2;
/*
//------------------------------------------------------------//
void manejador_senial(int a){
   
}
//------------------------------------------------------------//
*/


//------------------------------------------------------------//
void HILO1(void) {   
int s;
			s=pthread_mutex_lock(&mtx);        
	if (s!=0){
		printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);   }
	
	sleep(2);
	
	printf("\nHilo 1 de proceso A decrementa mutex\n");
		er=mq_send(descriptor_mq, MENSAJE_1, sizeof(MENSAJE_1+80), 2); 
		if (er!=0){
		printf("\nERROR al enviar MENSAJE_1 por COLA DE MENSAJE\n");
	exit(-1);   } 
		printf("\nMENSAJE_1 enviado por COLA DE MENSAJE\n");
	s=pthread_mutex_unlock(&mtx);      
	if (s!=0){
		printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);    }
	
	printf("\nHilo 1 de proceso A incrementa mutex\n");
	
   pthread_exit(0);
}
void HILO2(void) {   
int s;
			s=pthread_mutex_lock(&mtx);        
	if (s!=0){
		printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);   }
	
	sleep(2);
	
	printf("\nHilo 2 de proceso B decrementa mutex\n");

		er=mq_send(descriptor_mq, MENSAJE_2, sizeof(MENSAJE_2+80), 3);
		if (er!=0){
		printf("\nERROR al enviar MENSAJE_2 por COLA DE MENSAJE\n");
	exit(-1);   } 
		printf("\nMENSAJE_2 enviado por COLA DE MENSAJE\n");
	s=pthread_mutex_unlock(&mtx);      
	if (s!=0){
		printf("ERROR; pthread_mutex() = %d\n", s);
	exit(-1);    }
	
	printf("\nHilo 2 de proceso B incrementa mutex\n");
	



   pthread_exit(0);

}
//------------------------------------------------------------//


//------------------------------------------------------------//
int main() {

  
	 mq_unlink(MQ_PATH);
//--creo la cola de mensajes--//
      // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff1);
   attr.mq_maxmsg = 5;
    printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",attr_rcv.mq_curmsgs);
   // Se crea cola de mensajes
   descriptor_mq = mq_open(MQ_PATH, O_RDWR | O_CREAT | O_NONBLOCK, 0777, &attr); 
   if (descriptor_mq < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   printf("Cola de mensajes creada\n");

		//-- creo los procesos hijo --//
      switch (fork())
      {
            case -1:   // No pudo crear el proceso
                  printf ("Error. No se crea proceso hijo");
                  break;

            case 0:   //se crea proceso hijo A
                  printf ("C crea el proceso A\n");
                  printf ("Soy proceso A, Mi pid es %d \n", getpid());
			pthread_create(&th1, NULL, (void *)&HILO1, NULL);
                  while(1);
                  break;
      }
      
      switch (fork())
      {
            case -1:   // No pudo crear el proceso
                  printf ("Error. No se crea proceso hijo");
                  break;

            case 0:   //se crea proceso hijo B
                  printf ("C crea el proceso B\n");
                  printf ("Soy proceso B, Mi pid es %d \n", getpid());
			pthread_create(&th2, NULL, (void *)&HILO2, NULL);
                  while(1);
                  break;
      }
	pthread_join(hilo[0],NULL);
	pthread_join(hilo[1],NULL);
	 //-- espero a que terminen ambos procesos --//
      
	sleep(2);
	 // Se lee de cola de mensajes
   leido = mq_receive(descriptor_mq, buff1, attr_rcv.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()\n");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff1);
	
      //-- espero a que terminen ambos procesos --//
      wait(NULL);
      wait(NULL);

   pthread_exit(0);
}

//------------------------------------------------------------//

