/*
Tres procesos (A, B y c) se comunican por medio de dos Tuberia, del siguiente modo:
1) Proceso A escribe en la tuberia 1 "0123456789" y termina.
2) Proceso B espera que A termine y lee de la tuberia 1 lo escrito por el proceso A y lo muestra, 
3) Luego Proceso B escribe en la tuberia 2 "ABCDEFGHIJ" y termina.
4) El Proceso C espera que B termine y lee la tuberia 2 y muestra lo leido.
Considerar los procesos con la siguiente jerarquía: Proceso C (padre), Proceso B (hijo de C), Proceso A (hijo de B, nieto de C)
                                        ______________
1)Proceso A (escribe "0123456789") --> /_____1_______/--> 2)Proceso B espera que termine A y Lee tubería 1
3)Proceso B (escribe "ABCDEFGHIJ") --> /_____2_______/--> 4)Proceso C espera que termine B y Lee tubería 2
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

char buff[30];   

/*
//------------------------------------------------------------//
void manejador_senial(int a){
   
}
//------------------------------------------------------------//
*/

/*
//------------------------------------------------------------//
void HILO(void) {   


   pthread_exit(0);
}
//------------------------------------------------------------//
*/



int main(){

int pipe1[2], pipe2[2], proc;
int leido;
char buff[80];

pipe(pipe1); //dentro del main
pipe(pipe2);


	switch (fork()){ 
		
		case 0: //Proceso B, hijo de A
		    switch (fork()){ 

			case 0: //Proceso A, hijo de B, nieto de C
			printf("\nSoy el proceso A. Escribo en la tuberia 0123456789 y termino\n");
			//////escribe una tuberia desde un MENSAJE predeterminado
			close(pipe1[0]); //cierra descriptor para lectura, va a escribir
			strncpy(buff, MENSAJE_A, sizeof(MENSAJE_A)); 
			write(pipe1[1], buff, sizeof(MENSAJE_A));
			exit(0);
			break;

			default: //Proceso B, hijo de A
			printf("\nSoy el proceso B. Me bloqueo hasta que A escriba en la tuberia 1.\n");
			//////leer tuberia

			close(pipe1[1]); //cierra descriptor tuberia para escritura, puede leer
			leido = read(pipe1[0], buff, sizeof(buff)); //lee
			if(leido < 1){
			write (0, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
			}else {
			// print en consola
			printf("\nSoy el proceso B.\n");
			write (0, "\nLeido de la tuberia 1: ", sizeof("\nLeido de la tuberia 1: "));
			write (0, buff, leido-1);
			printf(", por el proceso pid %d \n", getpid());
 			}
			printf("\nSoy el proceso B. Escribo en la tuberia 2 mensaje B y termino.\n");
			//////escribe una tuberia desde un MENSAJE predeterminado
			close(pipe2[0]); //cierra descriptor para lectura, va a escribir
			strncpy(buff, MENSAJE_B, sizeof(MENSAJE_B)); 
			write(pipe2[1], buff, sizeof(MENSAJE_B));
			exit(0);
			break;

				   
			} // end fork B

		break;
		
		default: //Padre, Proceso C
			printf("\nSoy el proceso C. Me bloqueo hasta que B escriba en la tuberia 2.\n");
			//////leer tuberia

			close(pipe2[1]); //cierra tuberia para escritura, puede leer
			leido = read(pipe2[0], buff, sizeof(buff)); //lee
			if(leido < 1){
			write (0, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
			}else {
			// print en consola
			printf("\nSoy el proceso C.\n");
			write (0, "\nLeido de la tuberia 2: ", sizeof("\nLeido de la tuberia 2: "));
			write (0, buff, leido-1);
			printf(", por el proceso pid %d \n", getpid());
 			}
			printf("\nSoy el proceso C y termino, chau!\n");
			exit(0);
			break;
			

		break;


} //end primer fork



} //end main

