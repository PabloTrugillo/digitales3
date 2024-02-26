
/////////// final_47 /////////////////////
/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una cola de mensajes

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la cola de mensajes
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la cola de mensaje el mensaje "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso C lee la cola de mensaje y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|Cola de mensajes|--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------
 

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
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 

#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3" 


struct sockaddr_in direccion={};
int sockfd;

mqd_t descriptor_mq; 
struct mq_attr attr, attr_rcv;  


//------------------------------------------------------------//
void mnjs1(int puerto){	
   	sendto(sockfd, MENSAJE_A , sizeof(MENSAJE_A), 0, (struct sockaddr *) &direccion, sizeof(direccion));   
}
//------------------------------------------------------------//
void mnjs2(int b){
	mq_send(descriptor_mq, MENSAJE_B, sizeof(MENSAJE_B), 1);   
}
//------------------------------------------------------------//


//------------------------------------------------------------//


//------------------------------------------------------------//
int main(int argc, const char *argv[]) {
	printf("Soy el proceso A, mi pid es: %d\n",getpid());
    if (argc != 2){
       argv[1]="2000";   
   	} 

	//-- creo el socket del cliente --//
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   		exit(-1);     
   	} 
   	direccion.sin_family= AF_INET;
   	direccion.sin_port=htons(atoi(argv[1])); 
   	inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar
    //como el servidor esta en el mismo host usamos Localhost = 127.0.0.1
  	printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n", argv[1]);
   	printf ("El proceso A crea el socket datagrama cliente\n");


   	mq_unlink(MQ_PATH);// eliino cola de mnsaje x si habia otra creada de antes
	//--creo la cola de mensajes--//
    // Se fijan algunos parametros de la cola de mensajes antes de crearla
   	attr.mq_msgsize = sizeof(MENSAJE_B);
   	attr.mq_maxmsg = 5;
   
   	// Se crea cola de mensajes
   	descriptor_mq = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0777, &attr); 
   	if (descriptor_mq < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   	printf("El proceso A crea la cola de mensajes\n");

 	
	signal(SIGUSR1, mnjs1); 
	signal(SIGUSR2, mnjs2); 

	while(1);

  exit(0);
}

//------------------------------------------------------------//

