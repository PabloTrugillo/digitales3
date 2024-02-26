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
#define MEM_COM "MEM_COM"  //nombre
mqd_t mqd; 
struct mq_attr attr, attr_rcv;
int err, leido;
char buff[256];
int *ptr;
int error,largo,fd,leido;
char buffshared[1024];
struct stat sb;

void leercola(int c){

// Se lee de cola de mensajes
   leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()");
      exit(-1); }

   printf("\nMensaje leido: %s\n", buff);
}
void leermemoria(int d){
//--- Lee el estado de la memoria y se guarda en la estructura sb
//     el tamaño del archivo es el campo st_size
	error = fstat(fd, &sb);
	if (error == -1){
       printf("\nError en fstat\n");
       exit(-1); } 

//-- Copia  de la memoria compartida en buffshared

	memcpy(buffshared, ptr, sb.st_size);
	//printf("\nLeido de memoria compartida: %s", buffshared);
	write(STDOUT_FILENO,"Mensaje leido desde memoria compartida: ",sizeof("Mensaje leido desde memoria compartida: ")); 
	write(STDOUT_FILENO, buffshared, sb.st_size);
	write(STDOUT_FILENO,"\n",sizeof("\n"));	  


//return(0);
}
int main(){
signal(SIGTSTP, leercola);
signal(SIGINT, leermemoria);
// Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;

// Se abre cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1); }

//--- Abre la memoria compartida, y obtiene el descriptor
   fd = shm_open(MEM_COM , O_RDWR|O_CREAT, 0777 );
   if (fd == -1){
       printf("\nError en shm_open\n");
       exit(-1); }

//--- Se dimensiona la memoria y se pone a cero
   largo = 1024;   
   error = ftruncate(fd, largo);
   if (error == -1){
       printf("\nError en ftruncate\n");
       exit(-1); }


 //--- Se mapea la memoria compartida al espacio de memoria del proceso
 //    Devuelve un puntero al área reservada
   ptr = mmap(NULL, 10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
   if (ptr == (void *)-1){
       printf("\nError en mmap\n");
       exit(-1); }

while(1){}
}

