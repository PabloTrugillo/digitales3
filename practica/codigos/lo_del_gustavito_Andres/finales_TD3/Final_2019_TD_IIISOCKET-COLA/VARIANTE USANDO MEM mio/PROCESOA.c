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

#define MEM_COM "MEM_COM"  //nombre
#define MENSAJE "ABCDEFGHIJ"

int fd, error, leido, largo ;
int *ptr;
char buff1[1024];
struct stat sb;

int des_socket, sck_server, rx_socket, largo, cont,err;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;

void manejador_senial(int a){
//SE CIERRA EL SOCKET
   close(SockConexion);

//--- Borrar memoria compartida   
    error = shm_unlink(MEM_COM);
    if (error == -1){
       printf("\nError en shm_unlink\n");
       exit(-1); }
	
	printf("\nObjeto de memoria borrado\n");   
      
}

int main(int argc, const char *argv[])      {
signal(SIGINT,manejador_senial);
   if (argc != 2){
       argv[1]="2000"; } 

int leido;
   char buff[1024];   
  
   struct mq_attr attr, attr_rcv;

//--- Crea la memoria compartida, y obtiene el descriptor
   fd = shm_open(MEM_COM , O_RDWR|O_CREAT, 0777 );
   if (fd == -1){
       printf("\nError en shm_open\n");
       exit(-1); }

//--- Se dimensiona la memoria y se pone a cero
   largo = 1024;   
   error = ftruncate(fd, largo);
   if (error == -1){
       printf("\nError en ftruncate\n");
       exit(-1); }


 //--- Se mapea la memoria compartida al espacio de memoria del proceso
 //    Devuelve un puntero al área reservada
   ptr = mmap(NULL, 10, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0 );
   if (ptr == (void *)-1){
       printf("\nError en mmap\n");
       exit(-1); }

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

   if ((listen(SockEscucha, 1))<0) {
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
//LEE EL SOCKET Y MUESTRA EN PANTALLA

while (( rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx))) > 0) {  //lee del socket    
             // write ( SockConexion ,buffer_rx, rx_socket);         //escribe en socket
              write ( STDOUT_FILENO , "cliente:--> ", 12);         //escribe leyenda en pantalla
              write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
       

//--- Copia Mensaje en la memoria
    memcpy(ptr, MENSAJE, sizeof(MENSAJE));

    printf ("MENSAJE copiado en memoria\n"); 
  
}}}

}
