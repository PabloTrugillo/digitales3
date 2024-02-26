
/////////// final_46 /////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de un socket datagrama

1) El proceso D crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente, crea al proceso B y al proceso C 
3) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
4) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
5) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía al socket datagrama servidor
6) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía al socket datagrama servidor
7) El proceso D lee el socket datagrama y muestra lo leído en pantalla

El proceso D debe ser el primero en comenzar en una consola
El proceso A comienza luego de D otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Socket |    |   |    |        |  
       |     -------     | A |--->|Datagama|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

//Compile el programa: gcc -o final_46A final_46A.c -lpthread -lrt
//Ejecute el programa ./final_46A


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
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 

#define MENSAJE_1 "0123456789"
#define MENSAJE_2 "ABCDEFGHIJ"



int ipc_ab[2], ipc_ac[2];

//------------------------------------------------------------//
void mnjs1(int a){
	//cierro pipe que no se utiliza
	close(ipc_ac[0]);
	close(ipc_ac[1]);

	// Se cierra el lado de lectura
	close(ipc_ab[0]);
	// Se escribe en la tuberia
	write(ipc_ab[1], MENSAJE_1, sizeof(MENSAJE_1));
	printf("Proceso B envia MENSAJE_1\n");
	exit(0); 
}
//------------------------------------------------------------//
void mnjs2(int b){
	//cierro pipe que no se utiliza
	close(ipc_ab[0]);
	close(ipc_ab[1]);

   // Se cierra el lado de lectura
	close(ipc_ac[0]);
	// Se escribe en la tuberia
	write(ipc_ac[1], MENSAJE_2, sizeof(MENSAJE_2));
	printf("Proceso C envia MENSAJE_2\n");
	exit(0);
   
}
//------------------------------------------------------------//


//------------------------------------------------------------//


//------------------------------------------------------------//
int main(int argc, const char *argv[]) {

	int leido;
	char buff1[80], buff2[80];
	char buff_tcl[256], buff_sck[56];
   	struct sockaddr_in direccion={};
   	int sockfd, lee_tcl, lee_sck, conn;

   	if (argc != 2){
       argv[1]="2000";
   	} 
	printf ("Soy proceso A, Mi pid es %d \n", getpid());
 	pipe(ipc_ac);
 	pipe(ipc_ab);


//-- creo el socket del cliente --//
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd==-1) { 
       printf ("ERROR en funcion socket()\n");
   		exit(-1);     
   	} 
   	direccion.sin_family= AF_INET;
   	direccion.sin_port=htons(atoi(argv[1])); 
   	inet_aton("127.0.0.1", &direccion.sin_addr); //convierte la direccion IP a una cadena de caracteres en la notacion estandar
    //como el servidor esta en el mismo host usamos Localhost = 127.0.0.1
  	printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n",  argv[1]);
   	printf ("El proceso A crea el socket datagrama cliente\n");
	
//-- creo los procesos hijo --//
	switch (fork())
	{
		case -1:   // No pudo crear el proceso
			printf ("Error. No se crea proceso hijo");
			break;

		case 0:   //se crea proceso hijo B
			printf ("A crea el proceso B\n");
			printf ("Soy proceso B, Mi pid es %d \n", getpid());
			signal(SIGUSR1, mnjs1); 
			while(1);
			break;
	}
	
	switch (fork())
	{
		case -1:   // No pudo crear el proceso
			printf ("Error. No se crea proceso hijo");
			break;

		case 0:   //se crea proceso hijo C
			printf ("A crea el proceso C\n");
			printf ("Soy proceso C, Mi pid es %d \n", getpid());
			signal(SIGUSR2, mnjs2); 
			while(1);
			break;
	}
	//-- espero a que terminen ambos procesos --//
	wait(NULL);
	wait(NULL);

	//--leo el pipe ab y envío por socket--//
	printf("Termino proceso B\n");
	close(ipc_ab[1]);
	leido = read(ipc_ab[0], buff1, sizeof(buff1));
	if(leido < 1){
		write (0, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
	}else {			
		// Escribir en consola
		write (0, "Leido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
		write (0, buff1, leido);
		write (0, "\n", sizeof("\n"));
		sendto(sockfd, buff1, leido, 0, (struct sockaddr *) &direccion, sizeof(direccion));
	}

	//--leo el pipe ac y envío por socket--//
	printf("Termino proceso C\n");
	close(ipc_ac[1]);
	leido = read(ipc_ac[0], buff2, sizeof(buff2));
	if(leido < 1){
		write (0, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
	}else {			
		// Escribir en consola
		write (0, "Leido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
		write (0, buff2, leido);
		write (0, "\n", sizeof("\n"));
		sendto(sockfd, buff2, leido, 0, (struct sockaddr *) &direccion, sizeof(direccion));
	}
  exit(0);
}

//------------------------------------------------------------//

