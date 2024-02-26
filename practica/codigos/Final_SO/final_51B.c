/////////// final_51 /////////////////////
/*
Tres procesos (A,B,C) se comunican de la siguiente forma:
C y A se comunican por medio de un pipe
C y B se comunican por medio de una cola de mensajes

1) El proceso C crea el pipe 
2) El proceso C crea al proceso A.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la señal SIGUSR1 escribe en el pipe "0123456789" 
5) El proceso C cuando recibe la señal SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ" 
6) El proceso A lee el pipe y muestra en pantalla lo leído
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y muestra en pantalla lo leído

El proceso C es el encargado de crear el pipe y la cola de mensajes, es el primer proceso que se ejecuta.
Las señales se envían desde otra consola

 ---      -----------------      ---      --------  
|   |--->|     pipe        |--->| A |--->|Pantalla|  
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------  
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|  
 ---      -----------------      ---      --------
 
*/

//Compile el programa: gcc -o final_51B final_51B.c -lpthread -lrt
//Ejecute el programa ./final_51B

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
#include <netinet/in.h>
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
int main() {

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


  	exit(0);
}
//------------------------------------------------------------//


