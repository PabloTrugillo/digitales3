/////////// final_51 /////////////////////
/*
Tres procesos (A,B,C) se comunican de la siguiente forma:
C y A se comunican por medio de un pipe
C y B se comunican por medio de una cola de mensajes

1) El proceso C crea el pipe 
2) El proceso C crea al proceso A.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la señal SIGUSR1 escribe en el pipe "0123456789" 
5) El proceso C cuando recibe la señal SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ" 
6) El proceso A lee el pipe y muestra en pantalla lo leído
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y muestra en pantalla lo leído

El proceso C es el encargado de crear el pipe y la cola de mensajes, es el primer proceso que se ejecuta.
Las señales se envían desde otra consola

 ---      -----------------      ---      --------  
|   |--->|     pipe        |--->| A |--->|Pantalla|  
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------  
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|  
 ---      -----------------      ---      --------
 
*/

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name


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
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 



#define MQ_PATH "/MQ_TD3" 


//VARIABLES COLA DE MENSAJES

//Creo el descriptor de cola de mensajes
mqd_t messageQueue; 
int queueError;
struct mq_attr queueAttr;
char recieveBuffer[1024];  

int main() {
	
	
	printf ("-------------->Programa B<--------------\n");
	
	//***************************************************************************//		
	//**************************** Cola de Mensajes *****************************//	 
	//***************************************************************************//		
	
	// Se crea cola de mensajes
	messageQueue = mq_open(MQ_PATH, O_RDWR, 0777, NULL); 
	if (messageQueue < 0) {
		printf ("error en mq_open()");   
		exit(-1); }
	
	printf("Cola de mensajes creada o abierta\n"); 
	
	// Se leen parametros de la cola de mensajes
	if (mq_getattr(messageQueue, &queueAttr) == -1){
		printf ("error en mq_getattr()");
		exit(-1); }
	
	printf("Nro max. de mensajes en cola de mensajes: %ld\n",queueAttr.mq_maxmsg);
	printf("Longitud max. de mensaje: %ld\n",queueAttr.mq_msgsize);
	//printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",queueAttr.mq_curmsgs);
	
	while(1)
	{
		// Se leen parametros de la cola de mensajes
		if (mq_getattr(messageQueue, &queueAttr) == -1){
			printf ("error en mq_getattr()");
			exit(-1); }
		
		printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",queueAttr.mq_curmsgs);
		
		//Leo la cola de Mensajes
		queueError = mq_receive(messageQueue, recieveBuffer, queueAttr.mq_msgsize, 0);
		
		if (queueError < 0)
		{
			printf ("error en mq_receive()");
			exit(-1); 
		}
		
		//Imprimo por pantalla lo recibido
		printf("Mensaje leido de C: %s\n", recieveBuffer);
		
	}
	
	return(0);
}


