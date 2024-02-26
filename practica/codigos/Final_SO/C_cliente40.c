/* PROCESO C - SOCKET STREAM CLIENTE (TCP)
Tres procesos (A,B,C) No relacionados se comunican de la siguiente forma:
A y B se comunican por medio de una cola de mensajes
A y C se comunican por medio de un socket stream
1) El proceso A crea la cola de mensajes
2) El proceso A crea un socket stream servidor
3) El proceso C (cliente) crea un socket stream cliente 
4) El proceso C (cliente) cuando recibe la señal SIGTSTP (Ctrl+z) le envía al servidor el mensaje  "ABCDEFGHIJ"
5) El proceso B abre la cola de mensajes
6) El proceso A cuando recibe la señal SIGINT (Ctrl+c) cierra el socket y borra la cola de mensajes 
7) El proceso A (servidor) cuando recibe algo en por el socket lo lee lo muestra en pantalla y se lo envía al proceso B por medio de la cola de mensajes
8) Proceso B al recibir la señal SIGTSTP (Ctrl+z) lee de la cola de mensajes un mensaje y muestra lo leído

       ---              ---                ---  
      | C |            | A |              | B |
       ---              ---                ---
       |     ------     | |     ------     | |     --------  
       |--->|socket|--->| |--->|ColaMe|--->| |--->|pantalla|  
             ------             ------              --------

   c --> /_Socket_/--> A --> /_ColaMe_/ --> C --> /_Pantalla_/ 
*/

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

#define MENSAJE_A "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3"  // El Nombre de la cola de mensajes debe empezar con "/"
//#define MENSAJE_B "0123456789"

//-----------Definición de Variables Globales ----------------
   char buff_tcl[256], buff_sck[56]; //Se definen los string "buff_tcl" y buff_sck" para que actuen de buffer y almacenen los datos que se ingresan por teclado y que se leen del socket servidor respectivamente.
   struct sockaddr_in direccion={}; //Se define una estructura del tipo "sockaddr_in" con el nombre "direccion" para contener la Direccion del socket pasivo (servidor) al que se pretende conectar.
   int sockfd, lee_tcl, lee_sck, conn;

//---------Manejador de Señal------------
void manejador_sig(int a)
	{
		
	write (sockfd , MENSAJE_A, strlen(MENSAJE_A)+1); //Escribe en el socket cliente cuyo descriptor es "sockfd" el mensaje definido por "MENSAJE_A".
	write (STDOUT_FILENO , "\nSe envio MENSAJE_A al Proceso A\n", 33); //Escribe leyenda en pantalla (STDOUT_FILENO).
	
	}

//----------------- Inicio Main --------------------------

int main(int argc , char const * argv[]) ////La variable "argc" guarda el numero de argumentos que recibe el "main()". Es decir, cuando se ejecuta el programa "cliente.c" mediante ./cliente podemos incluir un argumento para el Numero de Puerto cuyo valor se guardará en la posición 1 del arreglo "argv". Por ejemplo: "./cliente 2000" 
   {
   int c=0;

   printf("\n\t\t----------- PROCESO C -----------\n");

//-------------- Señales -------------------------
   signal(SIGTSTP, manejador_sig);  //Asignamos un manejador (manejador_sig) a la señal SIGTSTP (Ctrl+Z).
   
   printf("Soy el Proceso C y mi PID = %d\n", getpid()); //EL proceso imprime su PID en pantalla.

//---------------- SOCKET ------------------------
   if (argc != 2) //Si el numero de argumentos recibidos por consola es distino de 2, significa que no especificamos ningun Puerto, por lo tanto se asigna el Puerto "2001".
   	{
   	argv[1]="2001";
   	} 

//****************** PASO 1 *******************//
 //-- socket(): Crear el socket -----------//

   sockfd=socket(AF_INET, SOCK_STREAM, 0); //La llamada a socket() devuelve en "sockfd" el descriptor del socket creado en caso de éxito ó -1 en caso de error. El argumento AF_INET (Via IPv4) especifica el "Dominio ó Familia" de la comunicación, el argumento SOCK_STREAM (TCP_con conexión) especifica el "Tipo de Socket" y el arguemnto 0 (por defecto) especifica el "Protocolo".

   if (sockfd==-1) //Si socket() devuelve "-1", hubo un ERROR al crear el socket.
	{ 
       	printf ("ERROR en funcion socket()\n");
   	exit(-1);     
	} 

   printf ("Paso 1: Se creo socket cliente\n");

//****************** PASO 2 *******************//

//-- preparar el address:port del host servidor------//Configuramos los campos de la estructura "direccion" que define la Direccion del socket pasivo (servidor) al que nos queremos conectar.
   direccion.sin_family= AF_INET; //Especifica el "Dominio ó Familia de la Dirección". AF_INET (via IPv4)
   direccion.sin_port=htons(atoi(argv[1])); //Especifica el "Numero de Puerto de Protocolo" al que se asocia el socket servidor.
//   inet_aton("localhost", &direccion.sin_addr); //convierte la direccion de Internet a una cadena de caracteres en la notacion estandar, Localhost = 127.0.0.1
   inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar. Como el servidor esta en el mismo host usamos Localhost = 127.0.0.1 y lo asignamos como "Dirección IP de la máquina (host)" a la que se asocia el socket servidor.
                                                                                                
   printf ("Cliente va a conectarse con IP:127.0.0.1 Puerto: %s\n",  argv[1]); //Se imprime el "Numero de Puerto" del socket pasivo (servidor) al que nos vamos a conectar.

//-- conectar el socket activo al socket de escucha --//
   conn=connect(sockfd, (struct sockaddr *)&direccion, sizeof (direccion)); //Se conecta el socket activo cuyo descriptor es "sockfd" al socket pasivo (servidor) cuya dirección está guardada en la estructura "direccion". El argumento "sockfd" especifica el descriptor del socket activo que va a hacer la peticion de conexion, el argumento "&direccion" especifica un puntero a la estructura de direccion del socket pasivo (servidor) y el argumento "sizeof (direccion)" especifica el Tamaño de dicha estructura. La función "connect()" devuelve en la variable "conn" un 0 en caso de éxito ó -1 si hay error.

   if (conn == -1) //Si "connect()" devolvió -1, hubo un error al intentar conectarse con el servidor.
	{   
        printf ("ERROR en funcion connect()\n");
   	exit(-1);     
	}  

   printf ("Paso 2: Connect(), cliente conectado\n");

//****************** PASO 3 *******************//
 
   while(1) //Bucle infinito para el intercambio de datos con el socket pasivo (servidor).
	{ 
   	
     	if (c == 0)
		{
		printf("\n\tIngrese Señal SIGTSTP (Ctrl+Z) para enviar MENSAJE_A al Socket...\n");
		}
	c = 1;
 	}
//****************** 4 *******************//
//------------cierrar la conexion --------// 

   close(sockfd); //Se cierra el socket cliente cuyo Descriptor es "sockfd".

   return 0;   
}

//Compilación: gcc -o C_cliente C_cliente.c
//Ejecución: ./C_cliente
//NOTA:

/*----------------COMENTARIOS--------------------
- Funcionamiento del programa
 
*/
