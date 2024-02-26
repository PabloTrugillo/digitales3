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

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3"
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
int leido,PID,i,control2=0,control1=0;
pid_t Hijo1,Hijo2;
char buff[1024];

mqd_t mqd; 
struct mq_attr attr, attr_rcv;
int err;

void manejador_msj1(int a){
control1=1;
}
void manejador_msj2(int b){
control2=1;
}

int main(){

signal(SIGUSR1, manejador_msj2);
signal(SIGUSR2, manejador_msj1);



   
   // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   printf("Cola de mensajes creada\n"); 

switch(fork()){

case 0:
Hijo1=getpid();
printf("pid hijo1 = %d \n",Hijo1);
while(control1==0){}
printf("Hola soy el Hijo1, mi pid es:%d\n",getpid());

// Escribe en cola de mensajes
   err = mq_send(mqd, MENSAJE2, strlen(MENSAJE2)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
      printf ("error en mq_send()");
      exit(-1);   }

   printf("Mensaje enviado (%d)\n", err);
break;

default:

	switch(fork()){
	case 0:
	Hijo2=getpid();
	while(control2==0){}
sleep(5);
	printf("Hola soy el Hijo2, mi pid es:%d\n",getpid());
	printf("pid hijo2 = %d \n",Hijo2);
	// Escribe en cola de mensajes
  	 err = mq_send(mqd, MENSAJE1, strlen(MENSAJE1)+1, 1);  //strlen nos da la longitud de una cadena
   	if(err == -1){
     	 printf ("error en mq_send()");
    	  exit(-1);   }
	printf("Mensaje enviado (%d)\n", err);exit(0);
	break;
	default:
	printf("Hola soy el Padre, mi pid es:%d, voy a enviar señal a mi Hijo2\n",getpid());
	
	kill(Hijo2,SIGUSR1);
	wait(NULL);
	
// Se lee de cola de mensajes

   leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff);

	}

printf("Hola soy el Padre, mi pid es:%d, voy a enviar señal a mi Hijo1\n",getpid());
kill(Hijo1,SIGUSR2);
wait(NULL);
// Se lee de cola de mensajes

   leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff);
// Se cierra cola de mensajes
   err = mq_close(mqd);
   if (( err < 0 )){
      printf ("error en mq_close()");
      exit(-1);   }

      printf("Cola de mensajes cerrada (%d)\n", err);
   
 // Se elimina cola de mensajes
   err = mq_unlink(MQ_PATH);
   if(err == -1){
      printf ("error en mq_unlink()");
      exit(-1);   }

      printf("Cola de mensajes eliminada (%d)\n", err);
exit(0);
}
exit(0);
}
