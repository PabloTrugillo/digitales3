/*
Tres procesos (A, B, C) se comunican por medio de una Cola de Mensaje.
El proceso A escribe en la Cola de Mensaje "0123456789" en forma atómica y termina.
El proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" en forma atómica y termina.
El proceso C espera a que los procesos A y B terminen, lee los mensajes de la cola de mensajes y los muestra.

**********************VERSIÓN PARA PROCESOS NO RELACIONADOS*************************

""ejecutar en distintas consolas primero el proceso A, luegoB y al final el C.""

*/

//Compile el programa: gcc -o final_muestra_A final_muestra_A.c -lpthread -lrt
//Ejecute el programa ./final_muestra_A


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
 mqd_t descriptor_mq;
      struct mq_attr attr,attr_rcv;
pthread_t hilo[5];

int rc, er;
int arre[5];
int leido;
char buff[80],buff1[80], buff2[80];
pthread_t th1, th2;


int main() {
		 printf ("Soy proceso A, Mi pid es %d \n", getpid());
			mq_unlink(MQ_PATH);
		//--creo la cola de mensajes--//
		// Se fijan algunos parametros de la cola de mensajes antes de crearla
		   attr.mq_msgsize = sizeof(buff1);
		   attr.mq_maxmsg = 5;
		    printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",attr_rcv.mq_curmsgs);
		   // Se crea cola de mensajes
		   descriptor_mq = mq_open(MQ_PATH, O_RDWR | O_CREAT | O_NONBLOCK , 0777, &attr); 
		   
		   if (descriptor_mq < 0) {
		      printf ("error en mq_open()");   
		      exit(-1); }
		      
		   printf("Cola de mensajes creada\n");
			
		er=mq_send(descriptor_mq, MENSAJE_1, sizeof(MENSAJE_1), 1); 
		if (er!=0){
		printf("\nERROR al enviar MENSAJE_1 por COLA DE MENSAJE\n");
		exit(-1);   } 
		
		printf("\nMENSAJE_1 enviado por COLA DE MENSAJE\n");
                 
exit(0);
}
