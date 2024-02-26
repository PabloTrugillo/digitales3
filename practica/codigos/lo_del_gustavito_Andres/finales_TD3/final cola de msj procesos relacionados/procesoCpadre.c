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
#define MQ_PATH "/MQ_TD3"  // El nombre de la cola de mensajes debe empezar con "/"
#define MENSAJE "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

int pidhijo1,pidhijo2,t=0,arreglo[2]={0};
 int err, leido;
  char buff[1024];   
  mqd_t mqd; 
  struct mq_attr attr, attr_rcv;

void manejador_senial1(int a){

// Escribe en cola de mensajes
   err = mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
      printf ("error en mq_send()");
      exit(-1);    }

   printf("Mensaje enviado (%d)\n", err);
//exit(0);
}
void manejador_senial2(int b){

// Escribe en cola de mensajes
   err = mq_send(mqd, MENSAJE_B, strlen(MENSAJE_B)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
      printf ("error en mq_send()");
      exit(-1);    }

   printf("Mensaje enviado (%d)\n", err);
}

int main() {
signal(SIGUSR1,manejador_senial1);
signal(SIGUSR2,manejador_senial2);
   
   
   // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   printf("Cola de mensajes creada\n"); 

//CREO A LOS HIJOS DEL PROCESO C PADRE
//for(i=0;i<=2;i++){
arreglo[t]=t;
switch(fork()){
case 0:
	printf("\n Hola soy el hijo %d y mi pid es %d\n",t,getpid());
	pidhijo1=getpid();
	//while(1){}
	
	break;
default:
wait(NULL);
kill(pidhijo1,SIGUSR1);


// Se lee de cola de mensajes
printf("\nHola soy el padre y voy a leer la cola de msj\n");
   leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff);

switch(fork()){
case 0:
	t++;
	printf("\n Hola soy el hijo %d y mi pid es %d\n",t,getpid());
	pidhijo2=getpid();
	//while(1){}
	//t++;
	break;
default:
wait(NULL);
kill(pidhijo2,SIGUSR2);


// Se lee de cola de mensajes
printf("\nHola soy el padre y voy a leer la cola de msj\n");
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
   
   //Se elimina cola de mensajes
   err = mq_unlink(MQ_PATH);
   if(err == -1){
      printf ("error en mq_unlink()");
      exit(-1);   }

      printf("Cola de mensajes eliminada (%d)\n", err);
break;
break;
}}
exit(0);
}
