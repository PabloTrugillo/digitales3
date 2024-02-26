/* 

Tres procesos (A, B y C) NO relacionados se comunican de la siguente forma.
1) El proceso A crea un socket stream servidor y queda a la espera de una conexión
2) El proceso A (servidor) crea un hilo cuando se conecta un cliente
3) El proceso B (cliente1) crea un socket stream cliente se conecta con el servidor
4) El proceso C (cliente2) crea un socket stream cliente se conecta con el servidor
5) El proceso A (servidor)cuando recibe algo en por el socket lo lee y muestra en pantalla
6) El proceso B (cliente1) cuando recibe la señal SIGINT le envía al servidor el mensaje  "0123456789"
7) El proceso C (cliente2) cuando recibe la señal SIGINT le envía al servidor el mensaje  "ABCDEFGHIJ"


Programa SERVIDOR socket STREAM (TCP) con HILOS
El servidor envia al socket lo que recibe del socket y muestra en pantalla lo recibido en el socket: Cliente:--> datos rx socket


Compilar: gcc -o procesoAservidor procesoAservidor.c -lpthread -lrt
Ejecutar: ./procesoAservidor nro_de_puerto_si_es_necesario.
			Ejemplo: ./procesoAservidor 2006

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>		//!!!


struct sockaddr_in struct_direccion={};		//variable "struct_direccion" del tipo estructura sockaddr_in
int SockEscucha, SockConexion;
pthread_t hilo_cliente[2];						//vector del tipo de 2 elementos
int rc, nro_hilo, nro_cliente;


void *manejador_cliente(void *desc_socket) {			//función "start" para los hilos de los clientes

int sock = *(int*)desc_socket;		//se realiza un CAST, es decir se le indica al compilador que interprete la variable referenciada por "desc_socket"
									//como un int
int rx_socket;
char buffer_rx[256];

   printf ("SERVIDOR crea hilo %ld para conexion con cliente %d \n", pthread_self(), nro_cliente);

///////////////////BUCLE RECEPCION DEL SERVIDOR y ESCRITURA en el SOCKET
   while ((rx_socket = read(sock, buffer_rx, sizeof (buffer_rx))) > 0) {  	//lee del socket "sock" y lo guarda en el buffer
																		//"buffer_rx", tantos bytes como lo indica "sizeof (buffer_rx)"
																		//read() devuelve la cantidad de bytes leidos
																		//IMPORTANTE BUCLE DE ESCRITURA EN EL SOCKET mientras el CLIENTE siga
																		//escribiendo, el SERVIDOR seguira entrando a este bucle
	write (sock, buffer_rx, rx_socket);         				//escribe en socket "sock", el contenido del buffer "buffer_rx",
															//tantos bytes como lo indica "rx_socket"
	//printf("Cliente %d --> ", nro_cliente);				//NO IMPRIME hasta que termino el hilo con Ctrl+C!!!
	write (STDOUT_FILENO , "cliente:--> ", 12);         		//escribe leyenda en pantalla
	write (STDOUT_FILENO , buffer_rx, rx_socket);     	//escribe en pantalla lo leido del socket "sock"
	printf("\n");
   } //cierro el while
   
   printf ("Hilo %ld termina\n", pthread_self());		//Se visualiza cuando se cierra el proceso CLIENTE, cerrando su consola.

   pthread_exit(NULL);
}


int main(int argc, const char *argv[]) {

   if (argc != 2) {
       argv[1]="2000"; 
   } 


 //****************** 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_STREAM, 0);		//dominio: AF_INET. Este dominio realiza la comunicacion entre aplicaciones que se 
															//ejecutan en los hosts conectados a través de una red IPv4. El dominio de comunicación 
															//indica dónde se encuentran los procesos que se van a intercomunicar. 
															//tipo: SOCK_STREAM
															//protocolo: 0 (protocolo por defecto)
															//Devuelve un descriptor de fichero en caso de éxito, ó -1 en caso de error.
   
   if(( SockEscucha)<0) {
	printf ("ERROR en funcion socket()\n");
	exit(-1);     
   } 
   
   printf ("Paso 1: SERVIDOR creo socket\n");

 //****************** 2 *******************//
//-- preparar el address:port -------------//llena la estructura
   struct_direccion.sin_family = AF_INET;						//AF_INET es una familia que trabaja con IPv4
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  	// asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     			// puerto

 //-- bind(): asociamos el socket a la direccion de modo que los clientes pueden localizar el socket.------//

   if (bind(SockEscucha, (struct sockaddr *)&struct_direccion, sizeof(struct sockaddr_in))<0) {		//puntero a una estructura que especifica la
																										//dirección a la que este socket se va a enlazar:
																										//(struct sockaddr *)&struct_direccion
																										//tamaño de la estructura: 
																										//sizeof(struct sockaddr_in)
	printf ("ERROR en funcion bind()\n");

   	exit(-1);     
   } 

   printf ("Paso 2: Asociar bind() \n");

 //****************** 3 *******************//
 //-- listen(): Permitir hasta 1 conexion pendiente --//para notificar al sistema operativo de su voluntad de aceptar conexiones entrantes.
			//marca el socket indicado en su argumento como un socket pasivo, es decir, como un socket que se utilizará para aceptar solicitudes 
			//de conexión entrantes

   if ((listen(SockEscucha, 1))<0) {						//número de conexiones pendientes del servidor: 1
       printf ("ERROR en funcion listen()\n");
   exit(-1);     } 

   printf ("Paso 3: Permitir conexiones listen()\n");

 //****************** 4 *******************//
 //-- accept(): se bloquea el proceso hasta que entre una conexion --//
   nro_hilo = 0;
   nro_cliente = 0;
   
   printf ("Paso 4: Bloqueo hasta que entre conexion accept()\n");
   while(1) {		//bucle infinito
   	SockConexion = accept(SockEscucha, NULL, 0) ;		//accept() acepta conexiones entrantes en un socket stream de escucha, crea un nuevo
															//socket y devuelve un descriptor de archivo para el socket ACEPTADO en caso de éxito,
															//o -1 en caso de error.

   	if(SockConexion >=0) {		//entra cuando algùn cliente hace connect(), es negativo cuando se finaliza el cliente o el cliente cierra el socket.
		printf ("Desbloqueo de accept(), entro conexion: %d\n", SockConexion);
   		send (SockConexion,"Bienvenido al servidor\n", 23, 0 ); 	//se transmite un mensaje a SockConexion

//--------------se crea un hilo para los clientes----------------
		
		nro_hilo++;				//nro de hilo, se incrementa por cada cliente que hace connect() y el servidor hace accept() 
      		if (nro_hilo>5) {nro_hilo=0;}
		nro_cliente++;

		rc = pthread_create(&hilo_cliente[nro_hilo], NULL, manejador_cliente, (void*) (&SockConexion));	//función "start": manejador_cliente
																												//argumento para la función "start":
																												//tipo de puntero generico a la dirección
																												//de memoria de SockConexion
		if (rc != 0) {
        		printf("ERROR; pthread_create() = %d\n", rc);
         		exit(-1);        
		}
	}	//cierro el if de SockConexion
   	
	else { printf ("Error en la conexion\n");  }			//accept() devuelve -1 en caso de error.

     //pthread_join(hilo_cliente[nro_cliente], NULL);//MAL!!! Nunca saldría de un hilo hasta que termine con Ctrl+C y recién ahí atendería al siguiente
   
   } //cierro while 1

 //****************** 5 *******************//
 //------------cerrar la conexion --------// 

   close(SockConexion); 


   return 0;
}
  