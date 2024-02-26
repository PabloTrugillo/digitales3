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

//Compile el programa: gcc -o final_fifoqA final_fifoqA.c -lpthread -lrt
//Ejecute el programa ./final_fifoqA


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

#define FIFO_PATH "/tmp/MI_FIFO"
#define MQ_PATH "/td3" 
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"

int leido,err,fifo1,fifo2,pid1,pid2,i;
char buff[100];

void FIFO1(int a)
{
	//mkfifo(FIFO_PATH, 0777);
	//fifo1 = open(FIFO_PATH, O_WRONLY, 0); // O_NONBLOCK
	/*write(buff, MENSAJE1, sizeof(MENSAJE1));
	write(fifo1, buff, sizeof(buff));*/
	//se escribe en el fifo el mnsaje arriba definido con su tamaño
	err = write(fifo1, MENSAJE1, sizeof(MENSAJE1));
	write(fifo1, buff, sizeof(buff));
	if(err == -1){
	write(STDOUT_FILENO, "\n error al escribir fifo \n", sizeof("\n error al escribir fifo \n"));
	return -1;
	}else{//si logra escribir, muestra lo escrito en pantalla
	write(STDOUT_FILENO, "\n escrito en fifo: \n", sizeof("\n escrito en fifo: \n"));
	write(STDOUT_FILENO, MENSAJE1, sizeof(MENSAJE1));
	exit(0);}
}

void FIFO2(int b)
{
	//mkfifo(FIFO_PATH, 0777);
	//fifo2 = open(FIFO_PATH, O_WRONLY, 0); // O_NONBLOCK
	/*write(buff, MENSAJE2, sizeof(MENSAJE2));
	write(fifo2, buff, sizeof(buff));*/
	//se escribe en el fifo el mnsaje arriba definido con su tamaño
	err = write(fifo2, MENSAJE2, sizeof(MENSAJE2));
	write(fifo2, buff, sizeof(buff));
	if(err == -1){
	write(STDOUT_FILENO, "\n error al escribir fifo \n", sizeof("\n error al escribir fifo \n"));
	return -1;
	}else{//si logra escribir, muestra lo escrito en pantalla
	write(STDOUT_FILENO, "\n escrito en fifo: \n", sizeof("\n escrito en fifo: \n"));
	write(STDOUT_FILENO, MENSAJE2, sizeof(MENSAJE2));
	exit(0);}
}





int main(int argc, const char *argv[])
{

//definicion senales
//signal(SIGUSR1,FIFO1);
//signal(SIGUSR2,FIFO2);

//Se crean FIFO
unlink(FIFO_PATH);    // si la FIFO existe la borro
mkfifo(FIFO_PATH, 0777);

fifo1 = open(FIFO_PATH, O_RDWR, 0); // O_NONBLOCK
fifo2 = open(FIFO_PATH, O_RDWR, 0); // O_NONBLOCK

//A crea la cola de mjes
int mqd;
struct sockaddr_in direccion={};
struct mq_attr attr, attr_rcv;

unlink(MQ_PATH);
   // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = 8192;
   attr.mq_maxmsg = 5;

mqd = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()\n");   
      exit(-1); }
      
   printf("Cola de mensajes creada\n");

//Se crean los hijos
pid1=fork();
if(pid1==0)
{
	printf("Hijo 1 creado, PID: %d\n ",getpid());
	signal(SIGUSR1,FIFO1);
	while(1){};

}else
{
	pid2=fork();
	if(pid2==0)
	{
		printf("Hijo 2 creado, PID: %d\n ",getpid());
		signal(SIGUSR2,FIFO2);
		while(1){};
	}else	//padre ejecuta
	{
	
	//wait(NULL);
	//wait(NULL);

	for(i=0;i<2;i++)
	{
		if(i==0)
		{	wait(NULL);
			leido = read(fifo1, buff, sizeof(buff));	//LEO DE FIFO Y GUARDO EN BUFF
			write (1, "Leido de la FIFO1 ", sizeof("Leido de la FIFO1 "));
            		write (1, buff, leido-1); //ESCRIBO BUFF POR PANTALLA	
            		//envio a cola de mensaje
            		err=mq_send(mqd, buff, strlen(buff)+1, 2);  //strlen nos da la longitud de una cadena
			if(err == -1){
        		write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
			   else {
				 write(STDOUT_FILENO, "\nMensaje enviado\n", sizeof("\nMensaje enviado\n"));   
            		printf("\nMensaje leido de FIFO1 y enviado a cola de mensaje por Proceso A(PID: %d)\n",getpid);}
		}
		
		if(i==1)
		{	wait(NULL);
			sleep(2);
			leido = read(fifo2, buff, sizeof(buff));	//LEO DE FIFO Y GUARDO EN BUFF
			write (1, "Leido de la FIFO2 ", sizeof("Leido de la FIFO2 "));
            		write (1, buff, leido-1); //ESCRIBO BUFF POR PANTALLA	
            		//envio a cola de mensaje
            		err=mq_send(mqd, buff, strlen(buff)+1, 1);  //strlen nos da la longitud de una cadena
            		if(err == -1){
        		write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
			   else {
				 write(STDOUT_FILENO, "\nMensaje enviado\n", sizeof("\nMensaje enviado\n"));   
            		printf("\nMensaje leido de FIFO1 y enviado a cola de mensaje por Proceso A(PID: %d)\n",getpid);}	
		}
	}
	
	//Se cierran FIFOS
	 close(fifo1);
	 close(fifo2);
	
	exit(0);
	
	}
}



}


















