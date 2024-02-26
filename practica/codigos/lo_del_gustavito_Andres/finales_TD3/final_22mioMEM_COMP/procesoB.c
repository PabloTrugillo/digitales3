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
int cont,s;
int *ptr;
#define semaforo "/sema1"
#define MEM_COM "MEM_COMP"
#define MENSAJE_A "0123456789"

int fd, ft, leido, largo, i,error;
int contador;
int *ptr;
char buff[1024];
struct stat sb;
char nroascii ;

void manejador_senial(int a){
 


//--- Copia Mensaje en la memoria
    memcpy(ptr, MENSAJE_A, sizeof(MENSAJE_A));
    printf ("MENSAJE copiado en memoria\n");
cont=0;

// Se incrementa el semaforo
	s=sem_post(sem);
	if (s != 0) {
		printf("ERROR sem_post()\n");
		exit(-1);     }
}

int main()   {   

signal(SIGINT,manejador_senial);


//------ crea el semaforo 
	sem = sem_open(semaforo,0);
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
if(cont==0){
printf("\n Proceso B esperando señal SIGINT para escribir en memoria\n");
cont++;
}
}
}
