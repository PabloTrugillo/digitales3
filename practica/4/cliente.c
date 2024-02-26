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

int main(int argc , char const * argv[])
{
	char MENSAJE[] = {0x0C,0x0D,0x0E,0x0D,0x0A,0x01,0x02,0x0B,0x06,0x09};
	char IP[] = {"127.0.0.1"};	
	
	int sockfd, conn, lee_sck;
	char buffer[MAX] = {0}; 
	int n = 0;
	char buff_tcl[256], buff_sck[56];
   struct sockaddr_in direccion={};
  
	// **** COMIENZO DE EJERCICIO
//-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 

   printf ("Paso 1: Se creo socket cliente\n");	
//****************** 2 *******************//

//-- preparar el address:port del host servidor------//
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(18080);   
   inet_aton("127.0.0.1", &direccion.sin_addr);
   
   printf ("Cliente va a conectarse con IP:127.0.0.1 Puerto: %s\n",  argv[1]);
   
//-- conectar el socket activo al socket de escucha --//
   conn=connect(sockfd, (struct sockaddr *)&direccion, sizeof (direccion));

   if (conn == -1) {   
        printf ("ERROR en funcion connect()\n");
   exit(-1);     }  

   printf ("Paso 2: Connect(), cliente conectado\n");
   
  	write ( sockfd , MENSAJE, sizeof(MENSAJE)); 
   
     lee_sck = read ( sockfd , buffer, sizeof (buffer));    //lee de socket 
     write (STDOUT_FILENO, "Servidor:--> ", 13);               //escribe leyenda en pantalla
     write (STDOUT_FILENO, buffer, lee_sck);       
   







	
	// *** FIN DE EJERCICIO
	
    printf("Cliente: recibidos %d bytes\n", n);            
    buffer[n] = '\0'; 
    printf("Cliente: respuesta %s\n", buffer); 

	close(sockfd);
	return 0;
}
