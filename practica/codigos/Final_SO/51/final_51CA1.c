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


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3" 




//VARIABLES COLA DE MENSAJES

//Creo el descriptor de cola de mensajes
mqd_t messageQueue; 
int queueError;
struct mq_attr queueAttr;
char recieveBuffer[1024];  

//VARIABLES IPC PIPE
int ipcPipe[2];
int recievedChars = 0;
char recieveBufferPipe[50] = {0};  


//SIGUSR1 Envío por PIPE a HIJO
void mnjs1(int a){
	
	//Escribe en el PIPE
	write(ipcPipe[1], MENSAJE_A, sizeof(MENSAJE_A));
	printf ("Mensaje enviado al HIJO: %s\n", MENSAJE_A);
	printf ("Padre Espera senial SIGUSR1 o SIGUSR2\n");
}
	
//SIGUSR2 Envío por COLA a B
void mnjs2(int b){
	
	//Envio mensaje por cola de mensajes a B
	// Escribe en cola de mensajes
	queueError = mq_send(messageQueue, MENSAJE_B, sizeof(MENSAJE_B), 1); 
	if(queueError == -1){
		printf ("error en mq_send()");
		exit(-1);    }
	
	printf("Mensaje enviado por Cola de Mensaje a B: %s (%d)\n", MENSAJE_B, queueError); 
	printf ("Padre Espera senial SIGUSR1 o SIGUSR2\n");
}
		
		
int main(int argc, const char *argv[]) {
	
	printf ("-------------->Programa CA<--------------\n");
	
	printf ("Mi pid es: %d\n", getpid());
	
	//***************************************************************************//		
	//*********************************** IPC ***********************************//	 
	//***************************************************************************//		
	
	if (pipe(ipcPipe) == -1)
	{
		printf ("Error. No se crea Pipe");
		exit(-1);
	}
	
	//***************************************************************************//		
	//**************************** Cola de Mensajes *****************************//	 
	//***************************************************************************//		
	
	// Se crea cola de mensajes
	messageQueue = mq_open(MQ_PATH, O_RDWR | O_CREAT | O_NONBLOCK, 0777, NULL); 
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
	
	//***************************************************************************//		
	//********************************** FORK ***********************************//	 
	//***************************************************************************//		
	
	switch (fork())
	{
		case -1:   // No pudo crear el proceso
			printf ("Error. No se crea proceso hijo");
		break;
			
		case 0:   //proceso hijo
			//Cierra escritura lado hijo
			close(ipcPipe[1]);
			printf ("\tCierro escritura PIPE hijo\n");
			
			while(1)
			{
				printf("\tHijo Leyendo PIPE...\n");
				
				//Espera lectura de la tuberia
				recievedChars = read(ipcPipe[0], recieveBufferPipe, sizeof(recieveBufferPipe));
				
				if(recievedChars < 1)
				{	//Si falla
					printf("\nError al leer tuberia");
				}
				else
				{//Si esta OK imprime el contenido de la tuberia
					printf("\tHijo lee de la tuberia: %s\n",recieveBufferPipe);
				}
			}
		break;
			
		default:  //proceso padre
		break;
	}
	
	
	
	//Cierra lectura lado padre
	close(ipcPipe[0]);
	printf ("Cierro lectura PIPE padre\n");
	
	//Si recibo la señal SIGUSR1 voy al manejador mnjs1
	signal(SIGUSR1, mnjs1);
	//Si recibo la señal SIGUSR2 voy al manejador mnjs2
	signal(SIGUSR2, mnjs2);
	
	printf ("Padre Espera senial SIGUSR1 o SIGUSR2\n");

	//***************************************************************************//		 
	//*********************************** Main **********************************//	 
	//***************************************************************************//	
	
	while(1)
	{
		
		sleep(100);
		
	}
}
		
		
		
		
		
		
		
		
		
		
		
