/* 
programa SOCKET SERVIDOR Stream (TCP) el servidor envia al socket
lo que recibe del socket y muestra en pantalla lo recibido en 
el socket: Cliente:--> datos rx socket
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
//primero la declaracion de variables 
int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;
// el argumento del main es una forma de recibir argumentos al momento de ejecutarlos en int argc queda la cantidad de argumentos que recibo despues de ejecutar, y los argumentos quedan guardados en el vector const char *argv[].
int main(int argc, const char *argv[])      {

   if (argc != 2){ // como el primer arg q recivo es el nombre entonces pregunto si la cantidad de argumentos es distinta a 2 de ser asi, supongo que es uno.
       argv[1]="2000"; } // ponemos 2000 xq va a ser el puerto que va a utilizar.


 //****************** 1 *******************//
 //-- socket(): Crear el socket -----------//
   SockEscucha=socket(AF_INET, SOCK_STREAM, 0);//guardo el descriptor en SockEscuch, SOCK_STREAM es el tipo de socket, AF_INET es el dominio y el protocolo en 0.  
   if(( SockEscucha)<0) {
       printf ("ERROR en funcion socket()\n");
   exit(-1);     } 
   
   printf ("Paso 1: Servidor creo socket\n");//si no hubo error , imprime el texto en pantalla.

 //****************** 2 *******************//

//-- preparar el address:port ------------- Antes de usar la funcion bind, vamos a rellenar la estructura con los campos, los tres campos q hay q llenar son: la familia,AF_INET xq va a trabajar en ipv4, luego la direccion ip, (INADDR_ANY) es una forma de decirle que tome el ip de la maquina que es donde va a correr el servidor, htonl es una transformacion de la ip al formato que necesita la estructura//
   struct_direccion.sin_family = AF_INET;
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     // puerto, el campo puerto se lo puedo pasar yo a la estructura o dejarlo x defecto con lo que habia asignado al vector (argv[1]), que en este caso era 2000

 //-- bind(): asociamos el socket a la direccion------//

   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) {//usamos la funcion bind para enlazar el descriptor del soquet con la direccion en el primer campo, en el segundo campo le pasamos el puntero a la estructura y en el tercero el tamaño de la estructura.
       printf ("ERROR en funcion bind()\n");
   exit(-1);     } 

   printf ("Paso 2: Asociar bind() \n"); //imprime en pantalla si no tuvo error

 //****************** 3 *******************//
 //-- listen(): Permitir hasta 1 conexion pendiente --//

   if ((listen(SockEscucha, 1))<0) { // con listen le decimos al socket que escuche, le pasamos el descriptor del socket y la cantidad de conexiones pendientes, 2 campos en total.
       printf ("ERROR en funcion listen()\n");
   exit(-1);     } 

   printf ("Paso 3: Permitir conexiones listen()\n");// si no hay error imprime en pantalla

  while(1) {

 //****************** 4 *******************//
 //-- accept(): se bloquea hasta que entre una conexion --//

   printf ("Paso 4: Bloqueo hasta que entre conexion accept()\n");// como voy a hacer un accept, el socket se va a bloquear por lo cual ante imprimo en pantalla que va a pasar esto.
   cont=0;  //declaramos un contador   
   SockConexion=accept(SockEscucha, NULL, 0) ;//hago el accept del socket q cree arriba, primer campo descriptor del socket, segundo le pongo NULL xq no me interesa recibir o guardar la direccion del cliente,

   if (SockConexion >=0) {  //SI ES LA PRIMERA VEZ…
      if (cont==0) {
           printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion);//EL DESCRIPTOR AHORA ES EL QUE ME DEVOLVIO ACCEPT. SE VA A QUEDAR ESCUCHANDO DE ESE SOCKET ENTONCES UTILIZ UNA FUNCION DE LECTURA ESTANDAR READ CON EL DESCRIPTOR DEL SOCKET, EL BUFFER DONDE QUIERO QUE GUARDE LOS DATOS Y EL TAMAÑO MAXIMO.
           send (SockConexion ,"Bienvenido al servidor\n", 23,0 ); 
      cont=1;  }   

      while (( rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx))) > 0) {  //lee del socket  //UNA VEZ QUE RECIBE ALGO LO ESCRIBE EN EL SOCKET, ESCRIBE EN EL DESCRIPTOR DEL SOCKET EL CONTENIDO DEL BUFFER RX QUE ES DONDE LO GUARDO Y EL TAMAÑO DE BYTES RECIBIDO. 
              write ( SockConexion ,buffer_rx, rx_socket);         //escribe en socket
              write ( STDOUT_FILENO , "cliente:--> ", 12);         //escribe leyenda en pantalla
              write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
       }
    } else { printf ("Error en la conexion\n");  }
 }

 //****************** 5 *******************//
 //------------cierrar la conexion --------// 

   close(SockConexion); 
 
  return 0;
}  
