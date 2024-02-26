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
#include <arpa/inet.h>
#define MENSAJE1 "0123456789"

int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;
int leido,control=0;
char buff[80];

void escribeSocket(int a){
control=1;
}

int main(int argc, const char *argv[])      {

signal(SIGTSTP,escribeSocket);

   if (argc != 2){
       argv[1]="2000"; } 

int ipc[2], proc;
	
pipe(ipc);

switch(fork()){
case 0:
printf("Hola soy el hijo, mi pid es: %d \n",getpid());
close(ipc[1]);
		
		leido = read(ipc[0], buff, sizeof(buff));
		if(leido < 1){
			write (0, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
		}else {
			
			// Escribir en consola
			write (0, "\nLeido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
			write (0, buff, leido-1);
			printf(", por el proceso hijo, pid %d \n", getpid());
		}



 //****************** 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_STREAM, 0);
   
   if(( SockEscucha)<0) {
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 
   
   printf ("Paso 1: Servidor creo socket\n");

 //****************** 2 *******************//

//-- preparar el address:port -------------//
   struct_direccion.sin_family = AF_INET;
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     // puerto

 //-- bind(): asociamos el socket a la direccion------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) {
       printf ("ERROR en funcion bind()\n");
   exit(-1);     } 

   printf ("Paso 2: Asociar bind() \n");

 //****************** 3 *******************//
 //-- listen(): Permitir hasta 1 conexion pendiente --//

   if ((listen(SockEscucha, 0))<0) {
       printf ("ERROR en funcion listen()\n");
   exit(-1);     } 

   printf ("Paso 3: Permitir conexiones listen()\n");

  while(1) {

 //****************** 4 *******************//
 //-- accept(): se bloquea hasta que entre una conexion --//

   printf ("Paso 4: Bloqueo hasta que entre conexion accept()\n");
   cont=0;     
   SockConexion=accept(SockEscucha, NULL, 0) ;

   if (SockConexion >=0) {  
      if (cont==0) {
           printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion);
	printf("Esperando señal SIGTSTP para escribir en socket\n"); }
	while(control==0){}
        write(SockConexion,buff,sizeof(buff));
	control=0;
	 
      cont=1;  } 
close(SockConexion);
exit(0);
}
break;
 
default:
printf("HOla soy el padre, voy a enviar mensaje a mi hijo \n");

// Se cierra el lado de lectura del padre
	close(ipc[0]);
// Se escribe en la tuberia
	write(ipc[1], MENSAJE1, sizeof(MENSAJE1));
	
	// Se cierra el lado de escritura. Se destruye la tuberia.
	close(ipc[1]);
wait(NULL);
exit(0);
}
}
