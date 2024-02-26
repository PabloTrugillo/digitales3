/*
 * Ejercicio 5 del TP de Cola de mensajes
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>

#define MENSAJE "DATA PARA PROCESO"
#define MQ_PATH "/MQ_TD3"  

   int err, leido;
   char buff[1024];   
   mqd_t mqd = 0; 
   struct mq_attr attr, attr_rcv;
   struct sigevent sev;   
   int retval;


void escribe_mensaje(int a){ //primer manejador de señal

   write(STDOUT_FILENO, "\nenviar mensaje\n", sizeof("\nenviar mensaje\n"));

   //-- Escribe en cola de mensajes --------------------------
   err = mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
        write(STDOUT_FILENO, "\nerror en mq_send()\n", sizeof("\nerror en mq_send()\n"));  }
   else {
        write(STDOUT_FILENO, "\nMensaje enviado\n", sizeof("\nMensaje enviado\n"));   }

}


void finaliza_proceso(int a){ //segundo manejador de señal

   err = mq_close(mqd);//cierra cola de mnsaje
   if (( err < 0 )){
      write(STDOUT_FILENO, "\nerror en mq_close()\n", sizeof("\nerror en mq_close()\n"));
      exit(0);   }

   write(STDOUT_FILENO, "\nCola de mensajes cerrada\n", sizeof("\nCola de mensajes cerrada\n"));

   err = mq_unlink(MQ_PATH);//elimina cola de mensaje
   if(err == -1){
      write(STDOUT_FILENO, "\nerror en mq_unlink()\n", sizeof("\nerror en mq_unlink()\n"));
      exit(0);  }

   write(STDOUT_FILENO, "\nCola de mensajes eliminada\n", sizeof("\nCola de mensajes eliminada\n"));

   write(STDOUT_FILENO, "\nTerminando proceso...\n", sizeof("\nTerminando proceso...\n"));

   exit(0);
}


int main() {//aca empieza y al iniciar coloca los dos manejadores de señal
   
   signal(SIGINT, escribe_mensaje);    //Ctrl + C
   signal(SIGUSR1, finaliza_proceso);  //termina

   printf("Soy el proceso: %d \n ",getpid());

   mq_unlink(MQ_PATH);//por precaucion elimina cualquier cola de mensaje que este declarada con el mismo nombre

   attr.mq_msgsize = sizeof(buff);//declara el atributo del tamaño de los mensajes, como maximo el tamaño de un buffer
   attr.mq_maxmsg = 5;//delcalra el atrib de cantidad max de mensajes
   
   // abrir y crear cola de mensajes con permisos de lectura y escrit pasandole los 4 campos,nombre, flags, permisos en octal y un puntero a la estructura de los atributos.
    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT , 0666, &attr);
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1);  }

   printf("Cola de mensajes creada\n");

 
  if (mq_getattr(mqd, &attr_rcv) == -1) {//le pasa los atributos a un descriptor y pregunta si da error
      printf ("error en mq_getattr()");
      exit(-1); }//a continuacion imprime los atributos
   printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);


   while(1){//hace una espera activa

       
   }  //fin while

   exit(0);//termina
}
