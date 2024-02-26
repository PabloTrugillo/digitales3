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

#define MEM_COM "MEM_COM"  //nombre
#define MENSAJE "INFORMACION PARA OTRO PROCESO\n"

int fd, error, leido, largo,PIDHIJO;
int *ptr;
char buff[1024];
struct stat sb;

void manejador(int a){

//printf("\n Soy el hijo, recibí señal de mi padre, a continuación voy a leer de la memoria\n");
write(0,"\nSoy el hijo, recibí señal de mi padre, a continuación voy a leer de la memoria",sizeof("\nSoy el hijo, recibí señal de mi padre, a continuación voy a leer de la memoria"));
//--- Lee el estado de la memoria y se guarda en la estructura sb
//     el tamaño del archivo es el campo st_size
	error = fstat(fd, &sb);
	if (error == -1){
       printf("\nError en fstat\n");
       exit(-1); }
       
//-- Copia  de la memoria compartida en buff
	memcpy(buff, ptr, sb.st_size);
	printf("\nLeido de memoria compartida: %s", buff);    
}

int main()  {   

signal(SIGUSR1,manejador);

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

//CREACION DE HIJO

switch(fork()){
	case 0:
	printf("\n Hola soy el hijo, mi pid es: %d\n",getpid());
	PIDHIJO=getpid();
	break;
	default:
	printf("\n Hola soy el Padre, mi pid es: %d\n",getpid());
	 //--- Copia Mensaje en la memoria
   	 memcpy(ptr, MENSAJE, sizeof(MENSAJE));
	 printf ("MENSAJE copiado en memoria\n"); 
	sleep(3); 
	kill(PIDHIJO,SIGUSR1);
	wait(NULL);

//--- Borrar memoria compartida   
    error = shm_unlink(MEM_COM);
    if (error == -1){
       printf("\nError en shm_unlink\n");
       exit(-1); }
	
	printf("\nObjeto de memoria borrado\n");
	break;
		}

 
    exit(0);
}
