/////////// FIFO_Queue /////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de una FIFO
A y C se comunican por medio de una FIFO
A y D se comunican por medio de una cola de mensajes

1) El proceso A crea la cola de mensajes, crea al proceso B y al proceso C 
2) El proceso B cuando recibe la señal SIGUSR1 escribe en el FIFO1 el mensaje "0123456789" y termina.
3) El proceso C cuando recibe la señal SIGUSR2 escribe en el FIFO2 el mensaje "ABCDEFGHIJ" y termina.
4) El proceso A espera a que termine el proceso B, lee el FIFO1 y lo leído lo envía a la cola de mensajes 
5) El proceso A espera a que termine el proceso C, lee el FIFO2 y lo leído lo envía a la cola de mensajes 
6) El proceso D lee los mensajes de la cola mensajes (2 mensajes) y los muestra en pantalla

El proceso A debe ser el primero en comenzar en una consola
El proceso D comienza luego de A otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| FIFO1 |--->|   |    | Cola   |    |   |    |        |  
       |     -------     | A |--->|Mensajes|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| FIFO2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

//Compile el programa: gcc -o final_fifoqB final_fifoqB.c -lpthread -lrt
//Ejecute el programa ./final_fifoqB


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

#define MQ_PATH "/td3" 


int main(int argc, const char *argv[])
{

   int err, leido;
   char buff[1024];   
   mqd_t mqd; 
   struct mq_attr attr, attr_rcv;

// Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDONLY | O_CREAT , 0666, &attr);
   if (mqd < 0) {
      printf ("error en mq_open()\n");   
      exit(-1); }
      
   printf("Cola de mensajes abierta\n"); 



   // Se leen parametros de la cola de mensajes
   if (mq_getattr(mqd, &attr_rcv) == -1){
      printf ("error en mq_getattr()\n");
      exit(-1); }
      
   printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);
	
   // Se lee de cola de mensajes
	 while(1) {
	leido = 0;
   //-- lee de cola de mensajes --------------------------
   /*leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);  
   if((leido < 0)){
        write(STDOUT_FILENO, "\nerror en mq_receive()\n", sizeof("\nerror en mq_receive()\n"));  }
   else {
	 write(STDOUT_FILENO, "\nleyendo mnsaj\n", sizeof("\n\nleyendo mnsaj\n"));
	sleep(2);
        write(STDOUT_FILENO, "\nMensaje leido exitosamente\n", sizeof("\nMensaje leido exitosamente\n"));
	write(STDOUT_FILENO, buff, leido); 
	write(STDOUT_FILENO, "\n", sizeof("\n")); }/*
		  }
   leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()\n");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff);*/
   
   // Se lee de cola de mensajes
    while(1) {
      
	leido = 0;
      	leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   	if (( leido < 0 )){
     	 printf ("error en mq_receive()");
    	  exit(-1); }

   	printf("Mensaje leido: %s\n", buff);

     	 sleep(1);

   }
   //----
  /*  leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()\n");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff);
   
   // Se lee de cola de mensajes
   leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()\n");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff);*/
   sleep(4);
   // Se cierra cola de mensajes
   err = mq_close(mqd);
   if (( err < 0 )){
      printf ("error en mq_close()\n");
      exit(-1);   }

      printf("Cola de mensajes cerrada (%d)\n", err);
   
   // Se elimina cola de mensajes
   err = mq_unlink(MQ_PATH);
   if(err == -1){
      printf ("error en mq_unlink()\n");
      exit(-1);   }

      printf("Cola de mensajes eliminada (%d)\n", err);









}

}
