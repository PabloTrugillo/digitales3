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



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <signal.h>
#include <semaphore.h>

#define MEM_COM "MEM_COM"  	//nombre del objeto de memoria compartida
#define semaforo "/sema1"			//nombre y ubicacion del semaforo en el sistema de archivos

int *ptr, s;			//puntero para la zona de memoria compartida
char buff[1024];
struct stat sb;		//estructura para la memoria compartida
sem_t *sem;


int main()  {   

int fd, error, leido, largo, sval;

//--- Borrar memoria compartida por si ya existe  
	error = shm_unlink(MEM_COM);
	if (error == -1){
		printf("El objeto de memoria no existe\n"); }
 
//IMPORTANTE!!! NOTAR LA UBICACION. ------Se elimina semaforo por si existe
	s=sem_unlink(semaforo);
	if (s != 0) {
		printf("ERROR sem_unlink()\n");   }

//--- Crea el OBJETO de memoria compartida, y obtiene el descriptor
   fd = shm_open(MEM_COM , O_RDWR|O_CREAT, 0777 );		//shm_open() devuelve descriptor de archivo en caso de éxito, o -1 si hay error.
   if (fd == -1){
       printf("\nError en shm_open\n");
       exit(-1); }

//--- Se dimensiona la memoria y se pone a cero
   largo = 1024;   
   error = ftruncate(fd, largo);	//ftruncate() trunca a un fichero a una longitud específica en bytes. Devuelde 0 si tuvo éxito, o -1 en caso de error
   if (error == -1){
       printf("\nError en ftruncate\n");
       exit(-1); }

 //--- Se mapea la memoria compartida al espacio de memoria del proceso
 //    Devuelve un puntero al área reservada
   ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );	//Mapea el objeto de memoria compartida en el espacio de memoria
																				//del proceso.
																				//Devuelve un puntero al área reservada o -1 en caso de error.
																				//start: NULL, entonces el kernel escoge una dirección adecuada
																				//para el mapeo.
																				//length: 10. Especifica el tamaño de la asignación en bytes.
																				//prot: PROT_READ | PROT_WRITE . Describe la protección de memoria 
																				//deseada, y no debe entrar en conflicto con el modo de apertura 
																				//del fichero.
																				//flag: MAP_SHARED. Comparte este área con todos los otros objetos 
																				//que señalan a este objeto
																				//file descriptor: fd
																				//offset: 0.
   if (ptr == (void *)-1){
       printf("\nError en mmap\n");
       exit(-1); 
	}

   printf ("\nDireccion de memoria local donde arranca la memoria %p\n", ptr);

//------ crea el semáforo con valor inicial 0!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!s
	sem = sem_open(semaforo, O_CREAT , 0777, 0);
	if (sem == SEM_FAILED) {
		printf("ERROR sem_open()\n");
		exit(-1);
	}
//------lee valor de semáforo 
	sem_getvalue(sem, &sval);
	printf("Valor de semaforo: %d\n", sval);

while(1) {	//bucle infinito de LECTURA
//intento decrementar el semáforo BLOQUEANDO este proceso LECTOR, queda a la ESPERA, para que el ESCRITOR pueda acceder a Mem Comp
	printf ("El proceso A decrementa el semaforo\n");
	s = sem_wait(sem);
	if (s != 0) {
		printf("ERROR sem_wait()\n");
		exit(-1);     }

//lee valor de semáforo. Aca vuelve a "despertarse" este proceso LECTOR gracias a sem_post en el ESCRITOR, decrementandose el semaforo
//que NO se pudo anteriormente
	sem_getvalue(sem, &sval);
	printf("Valor de semaforo: %d\n", sval);


//--- Lee el estado de la memoria y se guarda en la estructura sb
   error = fstat(fd, &sb);			//fstat() examina el archivo al que apunta el descriptor fd y rellena la estructura sb. 
									//Devuelde 0 si tuvo éxito, o -1 en caso de error
   if (error == -1){
	printf("\nError en fstat\n");
       	exit(-1); }

//-- Copia el contenido de la memoria compartida, apuntada por *ptr, en un buffer
   memcpy(buff, ptr, sb.st_size);							//el valor del campo st_size de la estructura sb, da el tamaño del archivo 
   printf("\nLeido de memoria compartida: %s\n", buff);	//muestra el contenido leído por pantalla
   } 	//cierro bucle infinito de LECTURA

   exit(0);
}