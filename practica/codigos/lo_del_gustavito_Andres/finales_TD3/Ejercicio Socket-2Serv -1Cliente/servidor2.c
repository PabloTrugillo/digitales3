#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;

int main(int argc, const char *argv[])      {

   if (argc != 2){
       argv[1]="2001"; } 


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
          write(SockConexion ,"Bienvenido al servidor2\n", 23); 
	rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx));
	write(STDOUT_FILENO,"Mensaje de cliente recibido: ",29);
	write (STDOUT_FILENO, buffer_rx, rx_socket);
      cont=1;  }   
	 	

      
    } else { printf ("Error en la conexion\n");  }
 }

 //****************** 5 *******************//
 //------------cierrar la conexion --------// 

   close(SockConexion); 
 
  return 0;
}  
