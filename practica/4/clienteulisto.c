/*
 * PARCIAL 4: proceso cliente TDP
 * 
 * Comando para compilar:
 * 		gcc -m32 -Wall -lpthread -lrt cliente.c -o cliente
*/
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define MAX 80
#define PORT 18080

#define SA struct sockaddr

int main()
{
	char MENSAJE[] = {0x0C,0x0D,0x0E,0x0D,0x0A,0x01,0x02,0x0B,0x06,0x09};
	char IP[] = {"127.0.0.1"};	
	
	int sockfd, conn, lee_sck;
	char buffer[MAX] = {0}; 
	int n = 0;
	char buff_tcl[256], buff_sck[56];
struct sockaddr_in direccion={};
	// **** COMIENZO DE EJERCICIO
//****************** 1 *******************//
//-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_DGRAM, 0);

   if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 

   printf ("Paso 1: Se creo socket cliente\n");	
   
//****************** 2 *******************//
//-- preparar el address:port del host servidor------//	
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(18080);
   inet_aton("127.0.0.1", &direccion.sin_addr);
   printf ("Cliente va a enviar datos a IP: %s Puerto: %s\n", IP, PORT);   
   
//-- envia al socket datos --

    sendto(sockfd, MENSAJE, sizeof(MENSAJE), 0, (struct sockaddr *) &direccion, sizeof(direccion));
    
//-- recibe datos del socket --    
   int len=sizeof(direccion);
   lee_sck = recvfrom(sockfd, buff_sck, sizeof (buff_sck), 0,(struct sockaddr *) &direccion,&len);
   write (STDOUT_FILENO, "Servidor:--> ", 13);               //escribe leyenda en pantalla
   write (STDOUT_FILENO, buff_sck, lee_sck);        //escribe lo leido del socket   
   
  
//****************** 3 *******************//
//------------cierrar la conexion --------// 

   close(sockfd);
 
   
   exit(0);
}
