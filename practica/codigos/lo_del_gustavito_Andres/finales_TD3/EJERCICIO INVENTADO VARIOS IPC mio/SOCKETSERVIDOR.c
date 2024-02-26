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
#define MEM_COM "MEM_COM"  //nombre
int *ptr;
int error,largo,fd,leido;
char buff[1024];
struct stat sb;
int des_socket, sck_server, rx_socket,cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion,err;
mqd_t mqd; 
struct mq_attr attr, attr_rcv; 
int t = 0 ;
int arre[5]={0};
pthread_t hilo[5];
int rc;

void manejador(int a){

//cierra la conexion --------// 
 close(SockConexion); 
 
// Se cierra cola de mensajes
   err = mq_close(mqd);
   if (( err < 0 )){
      printf ("error en mq_close()");
      exit(-1);   }

      printf("Cola de mensajes cerrada (%d)\n", err);
   
   // Se elimina cola de mensajes
   err = mq_unlink(MQ_PATH);
   if(err == -1){
      printf ("error en mq_unlink()");
      exit(-1);   }

      printf("Cola de mensajes eliminada (%d)\n", err);
//--- Borrar memoria compartida   
    error = shm_unlink(MEM_COM);
    if (error == -1){
       printf("\nError en shm_unlink\n");
       exit(-1); }
	printf("\nObjeto de memoria borrado\n");
exit(0);
}

void *hola(void * nro) {
   //sleep(3);
   printf("Hola, soy el hilo %d\n", * (int*) nro);

rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx)); //lee del socket    
              
write ( STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket

       

// Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = rx_socket;
   attr.mq_maxmsg = 5;
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   printf("\nCola de mensajes creada\n");

// Escribe en cola de mensajes
   err = mq_send(mqd, buffer_rx, strlen(buffer_rx)+1, 1);
   if(err == -1){
      printf ("error en mq_send()");
      exit(-1);    }

   printf("Mensaje enviado por cola de mensaje(%d)\n", err);


//--- Copia Mensaje en la memoria
    memcpy(ptr, buffer_rx, sizeof(buffer_rx));

    printf ("MENSAJE copiado en memoria\n");
}

int main(int argc, const char *argv[])      {


signal(SIGINT,manejador);
   if (argc != 2){
       argv[1]="2000"; } 


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
   ptr = mmap(NULL, 10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
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

   if ((listen(SockEscucha, 0))<0) {
       printf ("ERROR en funcion listen()\n");
   exit(-1);     } 

   printf ("Paso 3: Permitir conexiones listen()\n");

  while(1) {

 //****************** 4 *******************//
 //-- accept(): se bloquea hasta que entre una conexion --//

   printf ("\n Paso 4: Bloqueo hasta que entre conexion accept()\n");
   cont=0;     
   SockConexion=accept(SockEscucha, NULL, 0) ;

   if (SockConexion >=0) {  
      if (cont==0) {
           printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion);
           cont=1;  }   

//CREACION DE HILOS
	t++;  
	printf("Main creando el hilo nro %d\n", t);
    	arre[t] = t;
     
     //rc = pthread_create(&hilo[t], NULL, hola , (void *)(&t)  );
     rc = pthread_create(&hilo[t], NULL, hola , (void *) (&arre[t]) );
     
     if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);        }
    

  
/*//--- Lee el estado de la memoria y se guarda en la estructura sb
//     el tamaño del archivo es el campo st_size
	error = fstat(fd, &sb);
	if (error == -1){
       printf("\nError en fstat\n");
       exit(-1); } 
write(STDOUT_FILENO, ptr, sb.st_size);
*/
    } else { printf ("Error en la conexion\n");  }
 }


 
}  
