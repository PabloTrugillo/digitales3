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

#define MQ_PATH "/MQ_TD3"

int des_socket, sck_server, rx_socket, largo, cont,err;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;

void manejador_senial(int a){
//SE CIERRA EL SOCKET
   close(SockConexion);

// Se elimina cola de mensajes
   err = mq_unlink(MQ_PATH);
   if(err == -1){
      printf ("error en mq_unlink()");
      exit(-1);   }

      printf("Cola de mensajes eliminada (%d)\n", err);
      
}

int main(int argc, const char *argv[])      {
signal(SIGINT,manejador_senial);
   if (argc != 2){
       argv[1]="2000"; } 

int leido;
   char buff[1024];   
   mqd_t mqd; 
   struct mq_attr attr, attr_rcv;

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
       

// Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
// Se crea cola de mensajes

   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   printf("\n Cola de mensajes creada\n"); 

// Escribe en cola de mensajes

   err = mq_send(mqd, buffer_rx, strlen(buffer_rx)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
      printf ("error en mq_send()");
      exit(-1);    }

   printf("Mensaje enviado (%d)\n", err);
}}}

}
