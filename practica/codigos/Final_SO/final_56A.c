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

//Compile el programa: gcc -o final_56A final_56A.c -lpthread -lrt
//Ejecute el programa ./final_56A

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
//variables del socket
int SockEscucha, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
//variables de cola de ensajes 
int err, leido;
   char buff[1024];
   mqd_t mqd;
   struct mq_attr attr, attr_rcv;
//------------------------------------------------------------//
int main(int argc , char const * argv[]){
 
   if (argc != 2){
       argv[1]="2000";
   }      
	printf("Soy el Proceso A y mi PID = %d\n", getpid());
     mq_unlink(MQ_PATH);    //si la cola de mensaje existe la borro

	//---------------Cola de Mensajes---------------------------
// Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff); //Cambio el valor del Campo "mq_msgsize" (Tamaño Máximo del Mensaje) de la estructura "attr", por el tamaño definido por el string "buff[1024]".
   attr.mq_maxmsg = 5; //Cambio el valor del Campo "mq_maxmsg" (Cantidad Máxima de Mensajes) de la estructura "attr", a un valor máximo de 5 mensajes en la cola.
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0777, &attr); //Se abre la Cola de Mensajes con el Nombre definido por "MQ_PATH" y con los Flags (O_RDWR) para Lectura/Escritura y (O_CREAT) que permite crear la cola si no existe. El argumento "0777" indica los Permisos de Lectura, Escritura y Ejecución para el Usuario que la crea, para el Grupo al que pertenece el usuario y para el Resto de los Usuarios del sistema. Por último, el argumento "&attr" es un puntero a una estructura tipo "mq_attr" que especifica los nuevos atributos de la Cola de Mensajes. La función "mq_open()" devuelve el Descriptor de la Cola de Mensajes en caso de éxito ó -1 en caso de error.
   if (mqd < 0) //Hubo un ERROR al crear la cola de mensajes. mq_open() devuelve "-1".
	{
	printf ("error en mq_open()");   
      	exit(-1); //Termina el proceso con estado -1 (status=-1).
	}
      
   printf("Cola de mensajes creada\n");

 //****************** 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_DGRAM, 0);
   
   if(( SockEscucha)<0) {
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 
   
   printf ("Paso 1: creo socket servidor\n");

//----- preparar el address:port -------------//
   struct_direccion.sin_family = AF_INET;
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     // port pasado por argumento

 //****************** 2 *******************//
 //-- bind(): asociamos el socket a la direccion ------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) {
       printf ("ERROR en funcion bind()\n");
   exit(-1);     } 

   printf ("Paso 2: Asociar bind() \n");

//****************** 3 *******************//
//-- servidor espera a recibir algo ---
   while(1) {  //lee del socket    

     int len=sizeof(struct_direccion);
     rx_socket = recvfrom(SockEscucha, buffer_rx, sizeof (buffer_rx), 0,(struct sockaddr*)&struct_direccion,&len);            //recibe del socket 

     if (rx_socket>0) {
        write ( STDOUT_FILENO , "cliente:--> ", 12);      //escribe leyenda en pantalla
        write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
        write (0, "\n", sizeof("\n"));
	err = mq_send(mqd, buffer_rx, strlen(buffer_rx)+1, 1);  //strlen nos da la longitud de una cadena
      if(err == -1){
         printf ("error en mq_send()");
         exit(-1);}
 
      printf("Mensaje enviado (%d)\n", err);
     }
 }

 //****************** 4 *******************//
 //------------cierrar la conexion --------// 

   close(SockEscucha); 














  exit(0);
}

//------------------------------------------------------------//

