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

sem_t *sem;
#define semaforo "/sema1"
#define MEM_COM "MEM_COMP"
int main()   {   

int fd, ft, leido, largo, i,error,s;
int contador;
int *ptr;
char buff[1024];
struct stat sb;
char nroascii ;

//BORRO SEMAFORO SI EXISTE
s=sem_unlink(semaforo);
//------ crea el semaforo 
	sem = sem_open(semaforo, O_CREAT , 0666, 0);
	if (sem == SEM_FAILED) {
		printf("ERROR sem_open()\n");
		exit(-1);     }


//--- Crea la memoria compartida, y obtiene el descriptor
   fd = shm_open(MEM_COM , O_RDWR|O_CREAT, 0777 );
   if (fd == -1){
       printf("\nError en shm_open\n");
       exit(-1); }
   
   write (STDOUT_FILENO, "\nMemoria compartida creada", sizeof("\nMemoria compartida creada"));


//--- Se dimensiona la memoria y se pone a cero
   largo = 1024;   
   error = ftruncate(fd, largo);
   if (error == -1){
       printf("\nError en ftruncate\n");
       exit(-1); }

   write (STDOUT_FILENO, "\nMemoria compartida dimensionada", sizeof("\nMemoria compartida dimensionada"));

//--- Se mapea la memoria compartida al espacio de memoria del proceso
 //    Devuelve un puntero al área reservada
   ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );
   if (ptr == (void *)-1){
       printf("\nError en mmap\n");
       exit(-1); }

while(1){
//------decremento de semaforo 
	s = sem_wait(sem);
	if (s != 0) {
		printf("ERROR sem_wait()\n");
		exit(-1);     }

//--- Lee el estado de la memoria y se guarda en la estructura sb
//     el tamaño del archivo es el campo st_size
	error = fstat(fd, &sb);
	if (error == -1){
       printf("\nError en fstat\n");
       exit(-1); }
       
//-- Copia  de la memoria compartida en buff
	memcpy(buff, ptr, sb.st_size);
	printf("\nLeido de memoria compartida: %s\n", buff); 

/*------ Se incrementa el semaforo
	s=sem_post(sem);
	if (s != 0) {
		printf("ERROR sem_post()\n");
		exit(-1);     } */
}
}
