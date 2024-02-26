/*
Tres procesos (A, B, C) se comunican por medio de una Cola de Mensaje.
El proceso A escribe en la Cola de Mensaje "0123456789" en forma atómica y termina.
El proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" en forma atómica y termina.
El proceso C espera a que los procesos A y B terminen, lee los mensajes de la cola de mensajes y los muestra.

**********************VERSIÓN PARA PROCESOS NO RELACIONADOS*************************

""ejecutar en distintas consolas primero el proceso A, luegoB y al final el C.""


*/

//Compile el programa: gcc -o final_muestra_C final_muestra_C.c -lpthread -lrt
//Ejecute el programa ./final_muestra_C


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

int rc, er,i;
int arre[5];
int leido;
char buff[80],buff1[80], buff2[80];
pthread_t th1, th2;

int main() {

		 printf ("Soy proceso B, Mi pid es %d \n", getpid());
		sleep(2);
	 // Se lee de cola de mensajes
	 descriptor_mq = mq_open(MQ_PATH, O_RDWR,0777, &attr);
        
        // Se leen parametros de la cola de mensajes
   if (mq_getattr(descriptor_mq, &attr_rcv) == -1){
      printf ("error en mq_getattr()");
      exit(-1); }
      
   printf("Nro max. de mensajes en cola de mensajes: %ld\n",attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n",attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",attr_rcv.mq_curmsgs);
   
  // while(1){
	for(i=0;i<2;i++)
			{
			if(i==0)
			{
			   // Se lee de cola de mensajes
			   leido = mq_receive(descriptor_mq, buff, attr_rcv.mq_msgsize, 0);
			   if (( leido < 0 )){
			      printf ("error en mq_receive()");
			      exit(-1); }

			   printf("Mensaje leido: %s\n", buff);
				 //break;
				// while(1);
			      //-- espero a que terminen ambos procesos --//
			     // wait(NULL);
			     // wait(NULL);
			}
			if(i==1)
			{
			   // Se lee de cola de mensajes
			   leido = mq_receive(descriptor_mq, buff, attr_rcv.mq_msgsize, 0);
			   if (( leido < 0 )){
			      printf ("error en mq_receive()");
			      exit(-1); }

			   printf("Mensaje leido: %s\n", buff);
				 //break;
				// while(1);
			      //-- espero a que terminen ambos procesos --//
			     // wait(NULL);
			     // wait(NULL);
			}
}
  exit(0);
}
