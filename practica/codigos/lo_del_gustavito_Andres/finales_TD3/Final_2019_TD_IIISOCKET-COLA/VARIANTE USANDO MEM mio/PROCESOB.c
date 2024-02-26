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

#define MEM_COM "MEM_COM"  //nombre
int fd, error, leido, largo ;
int *ptr;
struct stat sb;
int err;
char buff1[1024];    
struct mq_attr attr, attr_rcv;

void manejador_senial3(int c){

//--- Lee el estado de la memoria y se guarda en la estructura sb
//     el tamaño del archivo es el campo st_size
	error = fstat(fd, &sb);
	if (error == -1){
       printf("\nError en fstat\n");
       exit(-1); }
       
//-- Copia  de la memoria compartida en buff
	memcpy(buff1, ptr, sb.st_size);
	printf("\nLeido de memoria compartida: %s", buff1); 
   
 exit(0);
}

int main() {
signal(SIGTSTP,manejador_senial3);   

//--- Crea la memoria compartida, y obtiene el descriptor
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
   ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );
   if (ptr == (void *)-1){
       printf("\nError en mmap\n");
       exit(-1); }  
   
  
while(1){
}
}
