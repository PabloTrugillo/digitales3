/*
Tres procesos NO relacionados (A, B y C) se comunican una Memoria Compartida.
1) El proceso A crea la Memoria compartida , y se queda en una espera activa.
2) El proceso B esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la Memoria compartida "0123456789" y vuelve a la espera activa.
3) El proceso C esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la Memoria compartida "ABCDEFGHIJ" y vuelve a la espera activa.
4) El proceso A lee la memoria compartida cada vez que escribe B o C y muesta lo leído.

Realizar la sincronización con semáforos con nombres, para que el proceso A se entere.

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
        printf("Llego senial, por lo tanto el proceso B ya escribio en memoria, soy el proceso A\r\n");
}
//------------------------------------------------------------//






int t, total, contador, s, longitud,sval,rc;
//------------------------------------------------------------//
int main() {

int fd, error, leido, largo ;
int *ptr;
char buff[1024];
struct stat sb;
char nroascii ;


 	//------Se elimina semaforo por si existe
	s=sem_unlink(semaforo);
	//--- Borrar memoria compartida por si ya existe  
	error = shm_unlink(MEM_COM);
		
  
	//------ crea el semaforo 
	sem = sem_open(semaforo, O_CREAT , 0777, 0);
	if (sem == SEM_FAILED) {
		printf("ERROR sem_open()\n");
		exit(-1);     }

	printf ("Main crea el semaforo\n");




 	//shm_unlink(MEM_COM); ///es para limpiar la memoria

   //--- Crea la memoria compartida, y obtiene el descriptor
	fd = shm_open(MEM_COM , O_RDWR|O_CREAT, 0777 );
	if (fd == -1){
		printf("Error en shm_open, %d,\n", fd);
		exit(-1); }

	printf("Objeto de memoria creado\n");

//--- Se dimensiona la memoria y se pone a cero
	largo = 1024;   
	error = ftruncate(fd, largo);
	if (error == -1){
		printf("Error en ftruncate %d\n", error);
		exit(-1); }

 //--- Se mapea la memoria compartida al espacio de memoria del proceso
 //    Devuelve un puntero al área reservada
	ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );
	if (ptr == (void *)-1){
		printf("Error en mmap %p\n", ptr);
		exit(-1); }

	printf("Memoria mapeada en: %p\n", ptr);

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
		printf("Error en fstat %d \n", error);
		exit(-1); }

//-- Copia  de la memoria compartida en buff
	memcpy(buff, ptr, sb.st_size);
	printf("\nLeido de memoria compartida: %s \n", buff);


/*//------decremento de semaforo 
	s = sem_wait(sem);
	if (s != 0) {
		printf("ERROR sem_wait()\n");
		exit(-1);     }   

//--- Lee el estado de la memoria y se guarda en la estructura sb
//     el tamaño del archivo es el campo st_size
	error = fstat(fd, &sb);
	if (error == -1){
		printf("Error en fstat %d \n", error);
		exit(-1); }

//-- Copia  de la memoria compartida en buff
	memcpy(buff, ptr, sb.st_size);
	printf("\nLeido de memoria compartida: %s \n", buff);*/
 }

}
