/*
* Ejercicio 1 del TP de Cola de mensajes
* Tipica implementacion de una cola de mensajes.
*/
//para compilar cola de mensajes se coloca -lrt, en este caso:    gcc -o mq01 mq01.c -lrt      y para ejecutar ./mq01  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <mqueue.h> //libreria de cola de mensajes

#define MENSAJE "DATA PARA OTROS PROCESOS"
#define MQ_PATH "/MQ_TD3"  // El nombre de la cola de mensajes debe empezar con "/"

int main() {
   
   int err, leido;
   char buff[1024];   
   mqd_t mqd; //aca guardo el descriptor de la cola de mnsaj
   struct mq_attr attr, attr_rcv; //aca guardo la estructura de la cola de mensajes
  
   // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff);// aca defino el tamaño maxim d mnsaje
   attr.mq_maxmsg = 5;// defino cantidad max de mnsajes de cola d mnsajes
   
   // Se crea cola de mensajes los parametros a pasar son nombre, bandera, permiso, estructura
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); //si existe la abre y si no, la crea y la abre 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   printf("Cola de mensajes creada\n"); 
   
   // Escribe en cola de mensajes, parametros:descriptor obtenido en open, mensaje, largo de mensaj y prioridad
   err = mq_send(mqd, MENSAJE, strlen(MENSAJE)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
      printf ("error en mq_send()");
      exit(-1);    }

   printf("Mensaje enviado (%d)\n", err);
   
   // Se leen parametros de la cola de mensajes
   if (mq_getattr(mqd, &attr_rcv) == -1){//esta fcion lee la estructura, le pasamos el descriptor y la estruc dnd quiero q guarde el valor leido.
      printf ("error en mq_getattr()");
      exit(-1); }
   //a continuacion se imprime parametros de la estructura   
   printf("Nro max. de mensajes en cola de mensajes: %ld\n",attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n",attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",attr_rcv.mq_curmsgs);

   //con esta funcion, se lee de cola de mensajes.parametros:num descriptor col mnsaj, bufer dnd voy a guardar, tamaño del mnsaje y el cero puede ser reemplazado por una variable entera que nos devuelva la prioridad del mensaje leido.
   leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
   if (( leido < 0 )){
      printf ("error en mq_receive()");
      exit(-1); }

   printf("Mensaje leido: %s\n", buff);
   
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
      
   exit(0);
}



//por defecto son bloqueantes, si un proceso trata de leer y no hay nada, se bloquea.
