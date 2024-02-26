/*
Tres procesos (A, B, C) se comunican por medio de una Cola de Mensaje.
El proceso A escribe en la Cola de Mensaje "0123456789" en forma atómica y termina.
El proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" en forma atómica y termina.
El proceso C espera a que los procesos A y B terminen, lee los mensajes de la cola de mensajes y los muestra.

**********************VERSIÓN PARA PROCESOS NO RELACIONADOS*************************

""ejecutar en distintas consolas primero el proceso A, luegoB y al final el C.""


*/

//Compile el programa: gcc -o final_muestra_B final_muestra_B.c -lpthread -lrt
//Ejecute el programa ./final_muestra_B


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

		 printf ("Soy proceso B, Mi pid es %d \n", getpid());
			sleep(1);
			descriptor_mq = mq_open(MQ_PATH, O_WRONLY | O_NONBLOCK , 0777, &attr);
			er=mq_send(descriptor_mq, MENSAJE_2, sizeof(MENSAJE_2), 2);
			if (er!=0){
		printf("\nERROR al enviar MENSAJE_2 por COLA DE MENSAJE\n");
	exit(-1);   } 
		printf("\nMENSAJE_2 enviado por COLA DE MENSAJE\n");
                 
exit(0);
}
