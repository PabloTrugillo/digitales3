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
#define MENSAJE_C "ABCDEFGHIJ"
#define semaforo "/sema1"			//nombre y ubicacion del semaforo en el sistema de archivos

int *ptr, sval, s;			//puntero para la zona de memoria compartida
sem_t *sem;

void manejador_SIGINT(int a){			//manejador de señal SIGINT

   printf("\nSeñal recibida por el proceso C\n");

//--- Copia Mensaje en la memoria
   memcpy(ptr, MENSAJE_C, sizeof(MENSAJE_C));	//Copia áreas de memoria
														//Destino: ptr	
														//Origen: MENSAJE
														//Longitud: sizeof(MENSAJE)

   printf ("MENSAJE copiado en memoria\n");

   s=sem_post(sem);		//INCREMENTA el valor del semáforo una unidad, para "despertar" al proceso LECTOR (proceso A)
   if (s != 0) {
	printf("ERROR sem_post()\n");
        exit(-1);     }


//------lee valor de semáforo 
   sem_getvalue(sem, &sval);
   printf("Valor de semaforo: %d\n", sval);

}


int main()  {   

int fd, error, leido, largo ;
char buff[1024];
struct stat sb;		//estructura para la memoria compartida

signal(SIGINT, manejador_SIGINT);  		//Disposicion para la senal SIGINT

//--- ABRE el OBJETO de memoria compartida, y obtiene el descriptor
   fd = shm_open(MEM_COM , O_RDWR, 0777);		//shm_open() devuelve descriptor de archivo en caso de éxito, o -1 si hay error.
   if (fd == -1){
       printf("\nError en shm_open\n");
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
  
//---- Se abre el semaforo creado por procesoA, los ultimos dos campos se ignoran porque el semáforo fue creado por dicho proceso
	sem = sem_open(semaforo, 0);		//o_flag: 0 (en lugar de O_CREAT). Se quiere acceder a un semáforo ya creado.
	if (sem == SEM_FAILED) {
		printf("ERROR sem_open()\n");
		exit(-1);
	}

//------lee valor de semáforo 
	sem_getvalue(sem, &sval);
	printf("Valor de semaforo: %d\n", sval);


  while(1){			//bucle infinito a la espera de senales

       
   }  //fin while

}