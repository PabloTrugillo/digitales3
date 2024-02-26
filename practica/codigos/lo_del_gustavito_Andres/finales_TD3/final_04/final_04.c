/*
Dos procesos (A y B) poseen una zona de Memoria Compartida, del siguiente modo:
Proceso A escribe en la Memoria Compartida "0123456789" y envía una señal al Proceso B.
El Proceso B al recibir la señal lee la Memoria Compartida y muestra lo leído.
--------------------------------------------

Considero:
Proceso A = Padre
Proceso B = Hijo

Compilar:
	gcc -o final_04 final_04.c -lrt
Ejecutar:
	
*/

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
#include <sys/wait.h>

#define MEM_COM "MEM_COM"	//nombre del objeto de memoria compartida
#define MENSAJE "0123456789"

int leer = 0;							//!!!
	
void manejador(int a){			//manejador de señal SIGUSR1
	leer=1;						//cambia el valor de "leer" para q salga del bucle
printf("\nSeñal recibida por el proceso Padre\n");
}


int main()  { 

char buff[1024];
struct stat sb;		//estructura para la memoria compartida
int error, fd;
int *ptr;			//puntero para la zona de memoria compartida;
int pid, leido, largo;

signal(SIGUSR1, manejador);	//La función signal() se utiliza para especificar qué se debe hacer cuando se recibe una señal determinada.

//--- Crea el OBJETO de memoria compartida, y obtiene el descriptor
   fd = shm_open(MEM_COM , O_RDWR|O_CREAT, 0777 );		//shm_open() devuelve descriptor de archivo en caso de éxito, o -1 si hay error.
   if (fd == -1){
       printf("\nError en shm_open\n");
       exit(-1); }

//--- Se dimensiona la memoria y se pone a cero
   largo = 1024;   
   error = ftruncate(fd, largo);				//ftruncate() trunca a un fichero a una longitud específica. Devuelde 0 si tuvo éxito, o -1 en caso de error
   if (error == -1){
       printf("\nError en ftruncate\n");
       exit(-1); }


pid = fork();	//Se crea un proceso Hijo

if (pid > 0) { 		//el Padre ejecuta:
	printf("Soy el proceso A, mi pid es %d, el pid de mi hijo es %d\n", getpid(), pid); 
	
	//--- Se mapea la memoria compartida al espacio de memoria del proceso
	 //    Devuelve un puntero al área reservada
   	ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );//Mapea el objeto de memoria compartida en el espacio de memoria
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
       		exit(-1); }
	printf ("Proceso A, direccion del puntero de memoria compartida: %p\n", ptr);   

	memcpy(ptr, MENSAJE, sizeof(MENSAJE));						      		 //El proceso Padre copia el mensaje en la memoria
  	printf ("MENSAJE copiado en memoria por el proceso A\n");


	
	kill(pid, SIGUSR1);															//El Padre envía una señal al Hijo para avisar que ya copió el mensaje
																				//en memoria...
	
	wait(NULL);     																//...y espera espera al proceso Hijo

	//--- El Padre borra la memoria compartida   
	error = shm_unlink(MEM_COM);			//shm_unlink() elimina un nombre de objeto de memoria compartida, y una vez que todos los
												//procesos han eliminado el objeto, libera y destruye el contenido de la zona de memoria asociada.
												//Devuelve 0 en caso de éxito, o -1 en caso de error.
    	if (error == -1){
       		printf("\nError en shm_unlink\n");
       		exit(-1); }
	
	printf("\nObjeto de memoria borrado\n");

	exit(0);
	}

if (pid == 0) {   //El proceso Hijo ejecuta:
	//signal(SIGUSR1, manejador);	//IMPORTANTE!!! Esta línea NO va dentro del proceso Hijo, debe ir afuera del mismo, dentro de main() en la
										//zona de definición de variables.

	printf("Soy el proceso B, mi pid es %d, el pid de mi padre es %d\n", getpid(), getppid());

	while(leer!=1){		//se inicializó leer=0 en la zona de declaración de variables
	
	}  					//se queda en un bucle hasta que recibe la señal SIGUSR1 del Padre, la cual cambia el valor de leer a 1

	//--- Se mapea la memoria compartida al espacio de memoria del proceso
    	//    Devuelve un puntero al área reservada
      	ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );//Mapea el objeto de memoria compartida en el espacio de memoria
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
       		exit(-1); }
	printf ("Proceso B, direccion del puntero de memoria compartida: %p\n", ptr);   

	//El Hijo lee el contenido de la memoria compartida
	//--- Lee el estado de la memoria y se guarda en la estructura sb
	error = fstat(fd, &sb);			//fstat() examina el archivo al que apunta el descriptor fd y rellena la estructura sb. 
									//Devuelde 0 si tuvo éxito, o -1 en caso de error
	if (error == -1){
		printf("\nError en fstat\n");
       		exit(-1); }

	//-- Copia el contenido de la memoria compartida, apuntada por *ptr, en un buffer
	memcpy(buff, ptr, sb.st_size);												//el valor del campo st_size de la estructura sb, da el tamaño del archivo 
	printf("\nLeido de memoria compartida por el proceso B: %s", buff);	//muestra el contenido leído por pantalla; 
																				//Leido de memoria compartida: Mensaje para proceso B.

	exit(0);
	}

}
