/////////// final_56 /////////////////////
/*
Tres procesos NO relacionados (A,B,C) se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensaje
A y C se comunican por medio de un socket datagrama 
1) El proceso A crea un socket datagrama servidor
2) El proceso A crea y abre la cola de mensajes con permisos de solo escritura 
3) El proceso B abre la cola de mensajes con permisos de solo lectura 
4) El proceso C crea un socket datagrama cliente 
5) El proceso C esta en una espera activa y cuando recibe la señal SIGUSR1 le envía al servidor el mensaje  "ABCDEFGHIJ"
6) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y lo escribe en la cola de mensajes
7) El Proceso B está en una espera activa y al recibir la señal SIGUSR1 leer la cola de mensajesy muestra lo leído

El proceso A debe ejecutarse primero
El proceso B debe ejecutarse segundo
El proceso C debe ejecutarse tercero
Las señales se envían desde otra consola

       ---              ---                  ---  
   S->| C |            | A |             S->| B |
       ---              ---                  ---
       |     ------     | |     --------     | |     --------  
       |--->|socket|--->| |--->|COLA MSJ|--->| |--->|pantalla|  
             ------             --------             --------

*/

//Compile el programa: gcc -o final_56C final_56C.c -lpthread -lrt
//Ejecute el programa ./final_56C


#define _GNU_SOURCE

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


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"
struct sockaddr_in direccion={};
int sockfd;

//------------------------------------------------------------//
void manejador_SIGUSR1(int a){

	sendto(sockfd, MENSAJE_B , sizeof(MENSAJE_B), 0, (struct sockaddr *) &direccion, sizeof(direccion));

}
//------------------------------------------------------------//


int main(int argc , char const * argv[]){
 
   if (argc != 2){
       argv[1]="2000";
   }      

	
	printf("Soy el proceso C, mi pid es: %d\n",getpid());
    if (argc != 2){
       argv[1]="2000";   
   	 

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
   	printf ("El proceso C crea el socket datagrama cliente\n");

	signal(SIGUSR1,manejador_SIGUSR1); 

	while(1);




	exit(0);
 }
}
//------------------------------------------------------------//

