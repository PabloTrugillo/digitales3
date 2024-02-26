/* PROCESO A - SOCKET STREAM SERVIDOR (TCP)
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


//----------- Definición de Variables Globales---------------------
//Variables Socket
   int des_socket, sck_server, rx_socket, largo, cont;
   char buffer_rx[256]; //Buffer para los datos que se leen del socket.
   struct sockaddr_in struct_direccion={}; //Se define una estructura del tipo "sockaddr_in" con el nombre "struct_direccion" para contener la Direccion a la cual se va a enlazar el socket.
   int SockEscucha,SockConexion; //Se definen las variables "SockEscucha" y "SockCOnexion" de tipo "int" para guardar los descriptores del socket "servidor" y del socket "cliente" respectivamente.

//Variables Cola de Mensajes
   int err, leido;
   char buff[1024]; //Se define una cadena de caracteres "buff[]" que voy a utilizar como memoria buffer para almacenar los mensajes leídos de la cola de mensajes.
   mqd_t mqd; //Defino una variable "mqd" que va a almacenar el Descriptor de la cola de mensajes.
   struct mq_attr attr, attr_rcv; //Defino dos estructuras tipo mq_attr: una es "attr" y la otra es "attr_rcv". La estructuras se crean con los valores de cada campo por defecto.

//---------Manejador de Señal------------
void manejador_sig(int a)
	{
	//****************** 5 *******************//
 	//------------ Cierra la Conexion --------// 
	int c;
   	c = close(SockConexion); //Se cierra el socket cliente cuyo Descriptor es "SockConexion".
	if (c == -1)
		{
		write(0, "\nError al cerrar Socket\n", sizeof("\nError al cerrar Socket\n")); //Escribe el "Texto" en la salida estándar (0), que es la pantalla.
		exit(-1); 
		}	
	write(0, "\nSe cerro el SOCKET\n", sizeof("\nSe cerro el SOCKET\n")); //Escribe el "Texto" en la salida estándar (0), que es la pantalla.
   	
	//----------- Cierra y Elimina Cola Mensajes ---------------//
	// Se cierra cola de mensajes
   	err = mq_close(mqd); //Se cierra la cola cuyo Descriptor es "mqd". El cierre de una cola de mensajes NO la Elimina.
   	if (( err < 0 )) //Hubo un error al cerrar la cola. mq_close() devuelve "-1".
		{
      		printf ("error en mq_close()");
      		exit(-1);   
		}
	write(0, "\nCola de mensajes cerrada\n", sizeof("\nCola de mensajes cerrada\n")); //Escribe el "Texto" en la salida estándar (0), que es la pantalla.
   	
   	// Se elimina cola de mensajes
   	err = mq_unlink(MQ_PATH); //La función mq_unlink() elimina la cola cuyo Nombre está definido por "MQ_PATH" y será destruida una vez que todos los procesos dejen de usarla.
   	if(err == -1) //Hubo un error al Eliminar la cola. mq_unlink() devuelve "-1".
		{
		write(0, "\nError en mq_unlink()\n", sizeof("\nError en mq_unlink()\n")); //Escribe el "Texto" en la salida estándar (0), que es la pantalla.
      		exit(-1); //Termina el proceso con estado -1 (status=-1).
		}
	write(0, "\nCola de mensajes Eliminada\n", sizeof("\nCola de mensajes Eliminada\n")); //Escribe el "Texto" en la salida estándar (0), que es la pantalla.	
	exit(0); //Termina el proceso con Estado 0 (status=0).
	
	}

//----------------- Inicio Main --------------------------

int main(int argc, const char *argv[]) //La variable "argc" guarda el numero de argumentos que recibe el "main(). Es decir, cuando se ejecuta el programa "servidor.c" mediante ./servidor podemos incluir un argumento para el Numero de Puerto cuyo valor se guardará en la posición 1 del arreglo "argv". Por ejemplo: "./servidor 2000"       
{

   printf("\n\t\t----------- PROCESO A -----------\n");

//--------------- Señales ---------------------------------
   signal(SIGINT, manejador_sig);  //Asignamos un manejador (manejador_sig) a la señal SIGINT (Ctrl+C).
   
   printf("Soy el Proceso A y mi PID = %d\n", getpid()); //EL proceso imprime su PID en pantalla.

//---------------Cola de Mensajes---------------------------
// Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff); //Cambio el valor del Campo "mq_msgsize" (Tamaño Máximo del Mensaje) de la estructura "attr", por el tamaño definido por el string "buff[1024]".
   attr.mq_maxmsg = 5; //Cambio el valor del Campo "mq_maxmsg" (Cantidad Máxima de Mensajes) de la estructura "attr", a un valor máximo de 5 mensajes en la cola.
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); //Se abre la Cola de Mensajes con el Nombre definido por "MQ_PATH" y con los Flags (O_RDWR) para Lectura/Escritura y (O_CREAT) que permite crear la cola si no existe. El argumento "0777" indica los Permisos de Lectura, Escritura y Ejecución para el Usuario que la crea, para el Grupo al que pertenece el usuario y para el Resto de los Usuarios del sistema. Por último, el argumento "&attr" es un puntero a una estructura tipo "mq_attr" que especifica los nuevos atributos de la Cola de Mensajes. La función "mq_open()" devuelve el Descriptor de la Cola de Mensajes en caso de éxito ó -1 en caso de error.
   if (mqd < 0) //Hubo un ERROR al crear la cola de mensajes. mq_open() devuelve "-1".
	{
	printf ("error en mq_open()");   
      	exit(-1); //Termina el proceso con estado -1 (status=-1).
	}
      
   printf("Cola de mensajes creada\n"); 

//----------------- SOCKET ----------------------------------
   if (argc != 2) //Si el numero de argumentos recibidos por consola es distino de 2, significa que no especificamos ningun Puerto, por lo tanto se asigna el Puerto "2001".
	{
       	argv[1]="2001"; 
	} 

 //****************** PASO 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_STREAM, 0); //La llamada a socket() devuelve en "SockEscucha" el descriptor del socket creado en caso de éxito ó -1 en caso de error. El argumento AF_INET (Via IPv4) especifica el "Dominio ó Familia" de la comunicación, el argumento SOCK_STREAM (TCP_con conexión) especifica el "Tipo de Socket" y el arguemnto 0 (por defecto) especifica el "Protocolo".
   
   if(( SockEscucha)<0) //Si socket() devuelve "-1", hubo un ERROR al crear el socket.
	{
       	printf ("ERROR en funcion socket()\n");
   	exit(-1);      
	} 
   
   printf ("Paso 1: Servidor creo socket\n");

 //****************** PASO 2 *******************//

//-- preparar el address:port -------------//Configuramos los campos de la estructura "struct_direccion" que define la Direccion que se va a enlazar a socket servidor.
   struct_direccion.sin_family = AF_INET; //Especifica el "Dominio ó Familia de la Dirección". AF_INET (via IPv4)
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY); //Especifica la "Dirección IP de la maquina (host)" a la que se asocia el socket. Para permitir conexiones de cualquier dirección se utiliza el valor "INADDR_ANY".
   struct_direccion.sin_port = htons(atoi (argv[1])); //Especifica el "Numero de Puerto de Protocolo" al que se asocia el socket.

 //-- bind(): asociamos el socket a la direccion------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion, sizeof(struct sockaddr_in))<0) //Se asocia el socket cuyo descriptor es "SockEscucha" con la Dirección especificada por la estructura "struct_direccion". El argumento "SockEscucha" contiene el Descriptor del socket servidor, el "&struct_direccion" es un Puntero a la estructura que especifica la direccion y el "sizeof(struct sockaddr_in)" especifia el Tamaño de la estructura tipo "sockaddr_in". La función "bind()" devuelve 0 en caso de éxito ó -1 si hay error.
	{
       	printf ("ERROR en funcion bind()\n"); //La función bind() devolvió "-1".
   	exit(-1);     
	} 

   printf ("Paso 2: Asociar bind() \n");

 //****************** PASO 3 *******************//
 //-- listen(): Permitir hasta 1 conexion pendiente --//

   if ((listen(SockEscucha, 1))<0) //Se especifica que el socket servidor cuyo descriptor es "SockEscucha" va a actuar como PASIVO, es decir que desea aceptar conexiones. El argumento "SockEscucha" especifica el Descriptor del socket pasivo y el argumento "1" indica el Backlog que define el numero máximo de conexiones pendientes, en este caso solo una. La funcion "listen()" devuelve 0 en caso de éxito y -1 en caso de error.
	{
       	printf ("ERROR en funcion listen()\n"); //La función "listen() devolvió -1.
   	exit(-1);     
	} 

   printf ("Paso 3: Permitir conexiones listen()\n");

  while(1) //Se define un lazo while para aceptar las peticiones de conexión recibidas.
	{

 //****************** PASO 4 *******************//
 //-- accept(): se bloquea hasta que entre una conexion --//

   	printf ("Paso 4: Bloqueo hasta que entre conexion accept()\n");
   	cont=0;     
   	SockConexion=accept(SockEscucha, NULL, 0); //Se aceptan conexiones entrantes al socket pasivo cuyo descriptor es "SockEscucha". El argumento "SockEscucha" especifica el descriptor del socket pasivo, el argumento "NULL" especifica un Puntero a la estructura de datos que contiene la Direccion asociada al socket pasivo y el argumento "0" especifica la Longitud de la estructura de Direccion. Como ya se enlazó la estructura de Direccion al socket "SockEscucha" no es necesario hacerlo nuevamente, por ello se coloca NULL y 0. La función "accept()" devuelve en "SockConexion" un entero No Negativo que es el Descriptor del socket aceptado (cliente) ó -1 en caso de error.

   	if (SockConexion >=0) //Si "accept()" tuvo éxito, se estableció la conexión con el cliente.
		{  
      		if (cont==0) 
			{
        		printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion); //Se imprime en pantalla el contenido de "SockConexion" que es el Descriptor del socket cliente. 
        		send (SockConexion ,"Bienvenido al servidor\n", 23,0 ); //Se envia un "mensaje" de bienvenida por única vez al socket cliente cuyo descriptor es "SockConexion".
      			cont=1; //Se modifica el valor de "cont" para que NO se vuelva a enviar el mensaje de bienvenida.  
			}   

       		while (( rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx))) > 0) //Se leen los datos del socket cliente cuyo Descriptor es "SockConexion" y se guardan en el string "buffer_rx[]". La función "read()" devuelve en la variable entera "rx_socket" el numero de bytes leidos si tuvo éxito la lectura ó -1 en caso de error.
			{    
                  	write (STDOUT_FILENO , "cliente:--> ", 12);         //Escribe leyenda en pantalla (STDOUT_FILENO).
                  	write (STDOUT_FILENO , buffer_rx, rx_socket);      //Escribe lo leido del socket cliente cuyo descriptor es "SockConexion".

			//-------------Cola de Mensajes---------------------
			// Escribe en cola de mensajes
  			err = mq_send(mqd, buffer_rx, strlen(buffer_rx)+1, 1);  //Se escribe en la cola de mensajes cuyo descriptor está en la variable "mqd". El mensaje a escribir o enviar esta definido por "buffer_rx" y su Longitud se calcula con "strlen(buffer_rx)+1". Por último, el cuarto argumento "1" especifica la Prioridad del mensaje. La función mq_send() devuelve "0" si tuvo éxto ó "-1" en caso de error.
   			if(err == -1) //Hubo un ERROR al enviar el mensaje. mq_send() devuelve "-1".
				{
      			  	printf ("error en mq_send()");
      			  	exit(-1); //Termina el proceso con estado -1 (status=-1).
   			  	}
 
   			printf("\nSe envio (%s) a la Cola de Mensajes\n", buffer_rx);

			printf("\n\tIngrese Señal SIGINT (Ctrl+C) para Cerrar Socket y Eliminar Cola Msj...\n"); 
          		}

	    	} 
	else //No se estableció la conexión con el cliente. "accept()" devolvió -1.
		{ 
           	printf ("Error en la conexion\n");  
           	}
 	}

 
}  

//Compilación: gcc -o A_servidor A_servidor.c -lrt
//Ejecución: ./A_servidor
//NOTA: Se utiliza el comando "-lrt" para que el compilador reconozca todas las funciones con el formato "mq_..." que hacen referencia al manejo de colas de mensajes.

/*----------------COMENTARIOS--------------------
- Funcionamiento del programa

*/
