/* PROCESO B - LEE COLA DE MENSAJES
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
   int err, leido;
   char buff[1024]; //Se define una cadena de caracteres "buff[]" que voy a utilizar como memoria buffer para almacenar los mensajes leídos de la cola de mensajes.
   mqd_t mqd; //Defino una variable "mqd" que va a almacenar el Descriptor de la cola de mensajes.
   struct mq_attr attr, attr_rcv; //Defino dos estructuras tipo mq_attr: una es "attr" y la otra es "attr_rcv". La estructuras se crean con los valores de cada campo por defecto.

//---------Manejador de Señal------------
void manejador_sig(int a)
    {
// Se lee de cola de mensajes
    leido = mq_receive(mqd, buff, attr.mq_msgsize, 0); //Se lee el mensaje de la cola cuyo Descriptor es "mqd" y se almacena en la cadena de caracteres apuntada por "buff". El tercer arguemento especifica el Tamaño Máximo (espacio disponible en bytes) para el mensaje y se coloca igual al Campo "mq_msgsize" (Tamaño Máx del Mensaje) de la estructura "attr" que define los atributos de la cola que estamos leyendo. Por último, el cuarto argumento es un puntero a un entero donde se va a Guardar la prioridad del mensaje leído; en este caso el "0" indica que NO voy a guardar la prioridad. La función "mq_receive()" devuelve el Numero de bytes recibidos ó "-1" en caso de error. 
    if (( leido < 0 )) //Hubo un ERROR al leer la Cola de Mensajes. mq_receive() devuelve "-1".
	{
	write(0, "Error en mq_receive()\n", sizeof("Error en mq_receive()\n")); //Escribe el "Texto" en la salida estándar (0), que es la pantalla.
       	exit(-1); //Termina el proceso con estado -1 (status=-1).
	}

    write (STDOUT_FILENO , "Mensaje Leido: ", sizeof("Mensaje Leido: "));
    write (STDOUT_FILENO , buff, sizeof(buff));
    write (STDOUT_FILENO , "\n", sizeof("\n"));

    }

//----------------- Inicio Main --------------------------
int main() 
{
   int c=0;

   printf("\n\t\t----------- PROCESO B -----------\n");

//-------------- Señales -------------------------
   signal(SIGTSTP, manejador_sig);  //Asignamos un manejador (manejador_sig) a la señal SIGTSTP (Ctrl+Z).
   
   printf("Soy el Proceso B y mi PID = %d\n", getpid()); //EL proceso imprime su PID en pantalla.

//-------------- Cola de Mensajes -----------------------

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
   
 
   if (mq_getattr(mqd, &attr_rcv) == -1) //Hubo un ERROR al leer los atributos de la cola. mq_getattr() devuelve "-1".
	{
      	printf ("error en mq_getattr()");
      	exit(-1); //Termina el proceso con estado -1 (status=-1).
	}
   //Se Imprimen en Pantalla los Atributos de la Cola de Mensajes   
   printf("Nro max. de mensajes en cola de mensajes: %ld\n",attr_rcv.mq_maxmsg); //Se imprime en pantalla el Campo "mq_maxmsg" (Cantidad Máx de Mensajes) de la estructura "attr_rcv".
   printf("Longitud max. de mensaje: %ld\n",attr_rcv.mq_msgsize); //Se imprime en pantalla el Campo "mq_msgsize" (Tamaño Máx del Mensaje) de la estructura "attr_rcv".
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",attr_rcv.mq_curmsgs); //Se imprime en pantalla el Campo "mq_curmsgs" (Cantidad de Mensajes en la cola Actualmente) de la estructura "attr_rcv".

   while(1)
	{
	
    	if (c == 0)
		{
		printf("\n\tIngrese Señal SIGTSTP (Ctrl+Z) para LEER Cola de Mensajes...\n");
		}
	c = 1;

	}

// Se cierra cola de mensajes
    err = mq_close(mqd); //Se cierra la cola cuyo Descriptor es "mqd". El cierre de una cola de mensajes NO la Elimina.
    if (( err < 0 )) //Hubo un error al cerrar la cola. mq_close() devuelve "-1".
	{
	
      	printf ("error en mq_close()");
      	exit(-1);   
	}

    printf("Cola de Mensajes Cerrada (%d)\n", err); //Se imprime en pantalla el valor devuelto por la función "mq_close()" que se guardó en la variable "err".
   
    // Se elimina cola de mensajes
    err = mq_unlink(MQ_PATH); //La función mq_unlink() elimina la cola cuyo Nombre está definido por "MQ_PATH" y será destruida una vez que todos los procesos dejen de usarla.
    if(err == -1) //Hubo un error al Eliminar la cola. mq_unlink() devuelve "-1".
	{
      	printf ("error en mq_unlink()");
      	exit(-1); //Termina el proceso con estado -1 (status=-1).
	}

    printf("Cola de Mensajes Eliminada (%d)\n", err); //Se imprime en pantalla el valor devuelto por la función "mq_unlink()" que se guardó en la variable "err".
      
    exit(0); //Termina el proceso con Estado 0 (status=0).
 
}

//Compilación: gcc -o B_lector B_lector.c -lrt
//Ejecución: ./B_lector
//NOTA: Se utiliza el comando "-lrt" para que el compilador reconozca todas las funciones con el formato "mq_..." que hacen referencia al manejo de colas de mensajes.

/*----------------COMENTARIOS--------------------
- Funcionamiento del programa
	
*/
