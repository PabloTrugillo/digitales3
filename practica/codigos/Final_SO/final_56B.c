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

//Compile el programa: gcc -o final_56B final_56B.c -lpthread -lrt
//Ejecute el programa ./final_56B

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

#define MQ_PATH "/MQ_TD3"
int err, leido;
char buff[1024];   
mqd_t mqd; 
struct mq_attr attr, attr_rcv;

//------------------------------------------------------------//
void manejador_SIGUSR1(int a){

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
//------------------------------------------------------------//
int main() {

int c=0;

   printf("\n\t\t----------- PROCESO B -----------\n");

//-------------- Señales -------------------------
   signal(SIGUSR1, manejador_SIGUSR1);  //Asignamos un manejador (manejador_sig) a la señal SIGTSTP (Ctrl+Z).
   
   printf("Soy el Proceso B y mi PID = %d\n", getpid()); //EL proceso imprime su PID en pantalla.

//-------------- Cola de Mensajes -----------------------

   // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff); //Cambio el valor del Campo "mq_msgsize" (Tamaño Máximo del Mensaje) de la estructura "attr", por el tamaño definido por el string "buff[1024]".
   attr.mq_maxmsg = 5; //Cambio el valor del Campo "mq_maxmsg" (Cantidad Máxima de Mensajes) de la estructura "attr", a un valor máximo de 5 mensajes en la cola.
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT, 0777, &attr); //Se abre la Cola de Mensajes con el Nombre definido por "MQ_PATH" y con los Flags (O_RDWR) para Lectura/Escritura y (O_CREAT) que permite crear la cola si no existe. El argumento "0777" indica los Permisos de Lectura, Escritura y Ejecución para el Usuario que la crea, para el Grupo al que pertenece el usuario y para el Resto de los Usuarios del sistema. Por último, el argumento "&attr" es un puntero a una estructura tipo "mq_attr" que especifica los nuevos atributos de la Cola de Mensajes. La función "mq_open()" devuelve el Descriptor de la Cola de Mensajes en caso de éxito ó -1 en caso de error.
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
		printf("\n\tIngrese Señal SIGUSR1 desde otra consola para LEER Cola de Mensajes...\n");
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




	
   
  exit(0);
}

//------------------------------------------------------------//

