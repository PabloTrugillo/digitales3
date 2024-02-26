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
//pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex

int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;
int rc,t=0,arreglo[2]={0},s,pid;


int main(int argc, const char *argv[])      {
//pthread_t hilo[1];

   
if (argc != 2){
       argv[1]="2000"; } 


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
if(cont==0){
   printf ("\nPaso 4: Bloqueo hasta que entre conexion accept()\n");
}
    
   SockConexion=accept(SockEscucha, NULL, 0) ;

if (SockConexion >=0) {  
cont++;   
   printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion);
 
//CREACION DE HIJO
t++;
arreglo[t]=t;
pid=fork();
if(pid==0){
rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx));  //lee del socket    
            
             printf("\n Hola soy el hijo %d\n",t);

              write ( STDOUT_FILENO , buffer_rx, rx_socket); 

	}

  }

}
}
