/*
Tres procesos NO relacionados (A, B y C) se comunican una Memoria Compartida.
1) El proceso A crea la Memoria compartida , y se queda en una espera activa.
2) El proceso B esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la Memoria compartida "0123456789" y vuelve a la espera activa.
3) El proceso C esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la Memoria compartida "ABCDEFGHIJ" y vuelve a la espera activa.
4) El proceso A lee la memoria compartida cada vez que escribe B o C y muesta lo leído.

Realizar la sincronización con semáforos con nombres.

*/

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name


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


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"
#define MEM_COM "MEM_COM"

sem_t *sem;
#define semaforo "/sema1"

int leer=0;
//------------------------------------------------------------//
void manejador_senial(int a){
	leer=1;
         printf("Llego senial, escribio en memoria, soy el proceso B\r\n");
	//memcpy(ptr, MENSAJE_A, sizeof(MENSAJE_A));
}
//------------------------------------------------------------//



int t, total, contador, s, longitud,sval,rc;


int main() {signal(SIGINT, manejador_senial);

int fd, error, leido, largo ;
int *ptr;
char buff[1024];
struct stat sb;
char nroascii ;

   

//------ abre el semaforo
	sem = sem_open(semaforo, 0);
	if (sem == SEM_FAILED) {
		printf("ERROR sem_open()\n");
		exit(-1);     }

	printf ("Main abre el semaforo\n");



 	//shm_unlink(MEM_COM); ///es para limpiar la memoria

//----- abre la memoria compartida, y obtiene el descriptor, NO LO CREO
	fd = shm_open(MEM_COM , O_RDWR, 0777 );
	if (fd == -1){
		printf("Error en shm_open, %d,\n", fd);
		exit(-1); }

	printf("Objeto de memoria abierto\n");

//--- Se mapea la memoria compartida al espacio de memoria del proceso
//    Devuelve un puntero al área reservada
	ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );
	if (ptr == (void *)-1){
		printf("Error en mmap %p\n", ptr);
		exit(-1); }

	printf("Memoria mapeada\n");

while(1){
while(leer == 0){}

	//--- Copia Mensaje en la memoria
   	memcpy(ptr, MENSAJE_A, sizeof(MENSAJE_A));

    	printf ("MENSAJE_A copiado en memoria\n");   

   	// printf ("Hacer >$cat /dev/shm/MEM_COM\n");

	//------ Se incrementa el semaforo
	s=sem_post(sem);
	if (s != 0) {
		printf("ERROR-2 sem_post()\n");
		exit(-1);     }
leer=0;

}  
  
}
