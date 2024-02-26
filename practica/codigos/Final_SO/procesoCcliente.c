/* 

Tres procesos (A, B y C) NO relacionados se comunican de la siguente forma.
1) El proceso A crea un socket stream servidor y queda a la espera de una conexión
2) El proceso A (servidor) crea un hilo cuando se conecta un cliente
3) El proceso B (cliente1) crea un socket stream cliente se conecta con el servidor
4) El proceso C (cliente2) crea un socket stream cliente se conecta con el servidor
5) El proceso A (servidor)cuando recibe algo en por el socket lo lee y muestra en pantalla
6) El proceso B (cliente1) cuando recibe la señal SIGINT le envía al servidor el mensaje  "0123456789"
7) El proceso C (cliente2) cuando recibe la señal SIGINT le envía al servidor el mensaje  "ABCDEFGHIJ"



Programa CLIENTE: socket STREAM

7) El proceso C (cliente2) cuando recibe la señal SIGINT le envía al servidor el mensaje  "ABCDEFGHIJ"

Servidor:--> datos rx socket 

Compilar: gcc -o procesoCcliente procesoCcliente.c
Ejecutar: ./procesoCcliente nro_de_puerto_de_cliente_si_es_necesario.
			Ejemplo: ./procesoCcliente 2006

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>	//!!!

#define MENSAJE_C "ABCDEFGHIJ"

int sockfd, lee_sck, conn;
char buff_sck[56];

void manejador_SIGINT(int a){			//manejador de señal SIGINT

   //printf("\nSeñal recibida por el proceso C. ");
   write (STDOUT_FILENO, "Señal recibida por el proceso C. ", 34);               	//escribe leyenda en pantalla

//****************** 3 *******************// TRANSMISION AL SERVIDOR
   
   write (sockfd , MENSAJE_C, sizeof(MENSAJE_C));		//!!! escribe en socket "CLIENTE", MENSAJE_B, tantos bytes como lo indica "sizeof()"
															//ESTO ES LO QUE RECIBE EL SERVIDOR

   lee_sck = read(sockfd , buff_sck, sizeof (MENSAJE_C));    	//lee de socket y lo guarda en el buffer "buff_sck". Es para imprimir luego por pantalla
																//el contenido escrito en el socket en lugar de imprimir por pantalla directamente el
																//mensaje enviado al Servidor.
   write (STDOUT_FILENO, "Servidor:--> ", 13);               	//escribe leyenda en pantalla
   write (STDOUT_FILENO, buff_sck, lee_sck);			//escribe en pantalla lo leido del socket, almacenado en "buff_sck", tantos bytes como lo
														//indica "lee_sck"
   printf("\n");
}


int main(int argc , char const * argv[]) {

   struct sockaddr_in direccion={};			//variable "direccion" del tipo estructura sockaddr_in


   signal(SIGINT, manejador_SIGINT);  		//Disposicion para la senal SIGINT

   if(argc != 2){
       argv[1]="2000";
   } 

//****************** 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd = socket(AF_INET, SOCK_STREAM, 0);		//dominio: AF_INET. Este dominio realiza la comunicacion entre aplicaciones que se 
														//ejecutan en los hosts conectados a través de una red IPv4. El dominio de comunicación 
														//indica dónde se encuentran los procesos que se van a intercomunicar. 
														//tipo: SOCK_STREAM
														//protocolo: 0 (protocolo por defecto)
														//Devuelve un descriptor de fichero en caso de éxito, ó -1 en caso de error.
   
   if(sockfd==-1) { 
	printf ("ERROR en funcion socket()\n");
   	exit(-1);     
   } 

   printf("Paso 1: CLIENTE creo socket\n");

//****************** 2 *******************//
//-- preparar el address:port del host servidor------//
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[1])); 
//inet_aton("localhost", &direccion.sin_addr);	//convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, 
												 	//Localhost = 127.0.0.1, otra forma de hacer lo de la línea de abajo
   inet_aton("127.0.0.1", &direccion.sin_addr); 	//convierte la direccion IP a una cadena de caracteres en la notacion estandar
                                                							//como el servidor esta en el mismo host usamos Localhost = 127.0.0.1, 
													//el servidor y cliente están en la misma PC
                                                                                                
   printf("Cliente va a conectarse con IP:127.0.0.1 Puerto: %s\n",  argv[1]);

//-- conectar el socket activo al socket de escucha pasivo--//
   conn=connect(sockfd, (struct sockaddr *)&direccion, sizeof (direccion));	//con connect() se hace el saludo de 3 vías
																				//Se debe especificar la dirección IP y el número de puerto (servicio) 
																				//del servidor al que se desea conectar, para esto se utiliza la estructura
																				//del tipo sockaddr_in de nombre "direccion"definida mas arriba
   if(conn == -1) {   
	printf ("ERROR en funcion connect()\n");
   	exit(-1);    
	 }  

   printf("Paso 2: Connect(), cliente conectado\n");

   
   lee_sck = read(sockfd , buff_sck, sizeof (buff_sck));    	//LECTURA del MENSAJE DE BIENVENIDA del SERVIDOR
															//lee de socket y lo guarda en el buffer "buff_sck"
															//NOTAR el argumento de sizeof(), quizás debería ser otra variable...

   write (STDOUT_FILENO, "Mensaje del Servidor: ", 22);               	//escribe leyenda en pantalla
   write (STDOUT_FILENO, buff_sck, lee_sck);			//escribe en pantalla lo leido del socket, almacenado en "buff_sck", tantos bytes como lo
														//indica "lee_sck" (probé con buff_sck y también funciona).


 while(1){			//bucle infinito a la espera de senales

       
   }  //fin while


//****************** 4 *******************//
//------------cierrar la conexion --------// 

   close(sockfd);

   return 0;   
}
