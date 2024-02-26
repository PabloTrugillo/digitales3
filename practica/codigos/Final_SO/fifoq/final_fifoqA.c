/////////// FIFO_Queue /////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de una FIFO
A y C se comunican por medio de una FIFO
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el FIFO1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el FIFO2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el FIFO1 y lo leído lo envía a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el FIFO2 y lo leído lo envía a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| FIFO1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| FIFO2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

//Compile el programa: gcc -o final_fifoqA final_fifoqA.c -lpthread -lrt
//Ejecute el programa ./final_fifoqA


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

#define FIFO_PATH1 "/tmp/MI_FIFO1"
#define FIFO_PATH2 "/tmp/MI_FIFO2"
#define MQ_PATH "/td3" 
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"

////////////////////////////////////////////////////////////////////////////////
mqd_t mqd;
struct mq_attr attr;
int fifo_d1, fifo_d2;
int cont1, cont2;
int leido1, leido2;
char buff[10], buff1[10], buff2[10];
//// Manejadores ///////////////////////////////////////////////////////////////////////////////////
void manj1(){
	write(fifo_d1, MENSAJE1, sizeof(MENSAJE1));
	write(STDOUT_FILENO,"Mensaje 1 enviado\n", sizeof("Mensaje 1 enviado\n"));
	exit(0);
	}

void manj2(){
	write(fifo_d2, MENSAJE2, sizeof(MENSAJE2));
	write(STDOUT_FILENO,"Mensaje 2 enviado\n", sizeof("Mensaje 2 enviado\n"));
	exit(0);
	}


int main(){
//// Crear MQ ///////////////////////////////////////////////////////////////////////////////////////
// Se fijan algunos parametros de la cola de mensajes antes de crearla
	attr.mq_msgsize = sizeof(buff);
	attr.mq_maxmsg = 5;
// Se crea cola de mensajes
	mq_unlink(MQ_PATH);
	mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
	if (mqd < 0) {
		printf ("error en mq_open()\n");   
		exit(-1); }
     
	printf("Cola de mensajes creada\n"); 
//// Crear Fifo /////////////////////////////////////////////////////////////////////////////////////
	unlink(FIFO_PATH1); 					//La borro si ya exite
	mkfifo(FIFO_PATH1, 0777);				//Crea la FIFO1
	fifo_d1 = open(FIFO_PATH1, O_RDWR, 0);  //Abre y devuelve descriptor
	
	unlink(FIFO_PATH2); 					//La borro si ya exite
	mkfifo(FIFO_PATH2, 0777);				//Crea la FIFO2
	fifo_d2 = open(FIFO_PATH2, O_RDWR, 0);  //Abre y devuelve descriptor
//// Configuro señales //////////////////////////////////////////////////////////////////////////////
	
	cont1=fork();							//Creo proceso B
	if(cont1==0){
	signal(SIGUSR1,manj1);
	printf("PID proceso B = %d\n", getpid());
	while(1);
	}
	
	cont2=fork();							//Creo proceso C
	if(cont2==0){
	signal(SIGUSR2,manj2);
	printf("PID proceso C = %d\n", getpid());
	while(1);
	}
	
	
/// Sigue  solo A////////////////////////////////////////////////////////////////////////////////////

	wait (NULL);
	
	leido1 = read(fifo_d1, buff1, sizeof(buff1));   //Devuelve la cantidad de bytes leidos de fifo1
	mq_send(mqd, buff1, sizeof(buff1), 0);
	
	leido2 = read(fifo_d2, buff2, sizeof(buff2));   //Devuelve la cantidad de bytes leidos de fifo2
	mq_send(mqd, buff2, sizeof(buff2), 0);

	//err = mq_close(mqd);	  //Cierra la mq
	
	//err = mq_unlink(MQ_PATH); //Elimina la mq

	sleep(1);

	return 0;
}
