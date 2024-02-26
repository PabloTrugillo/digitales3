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

//Compile el programa: gcc -o final_fifoqD final_fifoqD.c -lpthread -lrt
//Ejecute el programa ./final_fifoqD


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


struct mq_attr attr, attr_rcv;    //ver que borrar
int i, leido;
char buff[10];
mqd_t mqd; 

int main(){
		
	mqd = mq_open(MQ_PATH, O_RDONLY, 0777, &attr); 
	if (mqd < 0) {
		printf ("Error mq_open (%d).\n", mqd);    
		exit(-1);
		} 

	if (mq_getattr(mqd, &attr_rcv) == -1){
		printf ("Error en mq_getattr()\n");
		exit(-1); }
	for(i=0;i<2;i++){
	leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
	if (( leido < 0 )){
		printf ("error en mq_receive()\n");
		exit(-1); }
      
      
	printf("Mensaje leido: %s\n", buff);
	}
	return 0;
}
