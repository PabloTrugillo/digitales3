/* 
programa SOCKET SERVIDOR de datagramas (UDP) el servidor envia al socket
lo que recibe del socket y muestra en pantalla lo recibido en 
el socket: Cliente:--> datos rx socket
*/

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


//VARIABLES SOCKET

//Creo el descriptor de socket
int SocketServidor;
//Creo estructura direccion
struct sockaddr_in direccion = {};
//Variable auxiliares
int connectedSocket;
int recievedChars = 0;
char bufferSocket[50];

int main(int argc, const char *argv[])      
{
	printf ("-------------->Programa D<--------------\n");
	
   if (argc != 2){
       argv[1]="2000";
   } 

//***************************************************************************//		 
//********************************* Socket **********************************//	 
//***************************************************************************//	


//************************************ 1 ***********************************//
   
   //-- socket(): Crear el socket DATAGRAMA-----------//
   SocketServidor = socket(AF_INET, SOCK_DGRAM, 0);
   
   if (SocketServidor == -1) { 
	   printf ("ERROR en funcion socket()\n");
	   exit(-1);     } 
   
   printf ("Paso 1 Socket: Se creo socket servidor\n");
   
   
   //-- preparar el address:port del host servidor STREAM ------//
   //Tipo de red
   direccion.sin_family = AF_INET;
   
   //Transformo el puerto
   direccion.sin_port = htons(atoi(argv[1])); 
   
   // asigna una IP de la maquina
   direccion.sin_addr.s_addr = htonl(INADDR_ANY);
   
   
   
//************************************ 2 ***********************************//
   
   //-- bind(): asociamos el socket a la direccion ------//
   
   if (bind (SocketServidor, (struct sockaddr *)&direccion,sizeof(struct sockaddr_in))<0) {
	   printf ("ERROR en funcion bind()\n");
	   exit(-1);     } 
   
   printf ("Paso 2: Asociar bind() \n");
   
//************************************ 3 ***********************************//
   
   while(1){ 
	   
	   //-- recibe datos del socket -- 
	   int len = sizeof(direccion);
	   recievedChars = recvfrom(SocketServidor, bufferSocket, sizeof (bufferSocket), 0,(struct sockaddr *) &direccion, &len);
	   
	   //Imprimo por pantalla lo recibido del socket
	   printf ("Recibido de Cliente A: %s\n", bufferSocket);
	   
   }
   
   //****************** 3 *******************//
   //------------cierrar la conexion --------// 
   
   close(SocketServidor);
   
  return 0;
}  
