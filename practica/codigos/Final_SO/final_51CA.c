/////////// final_51 /////////////////////
/*
Tres procesos (A,B,C) se comunican de la siguiente forma:
C y A se comunican por medio de un pipe
C y B se comunican por medio de una cola de mensajes

1) El proceso C crea el pipe 
2) El proceso C crea al proceso A.
3) El proceso C crea la cola de mensajes
4) El proceso C cuando recibe la señal SIGUSR1 escribe en el pipe "0123456789" 
5) El proceso C cuando recibe la señal SIGUSR2 escribe en la cola de mensajes "ABCDEFGHIJ" 
6) El proceso A lee el pipe y muestra en pantalla lo leído
7) El proceso B abre la cola de mensajes
8) El proceso B lee el mensaje de la cola de mensajes y muestra en pantalla lo leído

El proceso C es el encargado de crear el pipe y la cola de mensajes, es el primer proceso que se ejecuta.
Las señales se envían desde otra consola


 ---      -----------------      ---      --------  
|   |--->|     pipe        |--->| A |--->|Pantalla|  
| C |     -----------------      ---      --------
|   |     -----------------      ---      --------  
|   |--->|Cola de mensajes |--->| B |--->|Pantalla|  
 ---      -----------------      ---      --------
 
*/

//Compile el programa: gcc -o final_51CA final_51CA.c -lpthread -lrt
//Ejecute el programa ./final_51CA


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
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 


#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

#define MQ_PATH "/MQ_TD3" 
int ipc_ac[2];
int err;
mqd_t descriptor_mq;
int leido;
      char buff1[80], buff2[80];
      char buff_tcl[256];
      int lee_tcl;
  int    a=0;
      struct mq_attr attr;
//------------------------------------------------------------//
void mnjs1(int a){
	//cierro pipe que no se utiliza
      
     

   // Se cierra el lado de lectura
      close(ipc_ac[0]);
      // Se escribe en la tuberia
      write(ipc_ac[1], MENSAJE_A, sizeof(MENSAJE_A));
      printf("Proceso C envia MENSAJE_A\n");
	a=1;
      exit(0);

}
//------------------------------------------------------------//
void mnjs2(int b){
	// Escribe en cola de mensajes
   err = mq_send(descriptor_mq, MENSAJE_B, strlen(MENSAJE_B)+1, 1);  //strlen nos da la longitud de una cadena
   if(err == -1){
      printf ("error en mq_send()");
      exit(-1);   }

   printf("Mensaje enviado (%d)\n", err);
//break;

}
//------------------------------------------------------------//


//------------------------------------------------------------//
int main(int argc, const char *argv[]) {

 

	signal(SIGUSR1, mnjs1);
	signal(SIGUSR2, mnjs2);

      if (argc != 2){
       argv[1]="2000";
      } 
      printf ("Soy proceso C, Mi pid es %d \n", getpid());
      pipe(ipc_ac);
		//-- creo el proceso hijo --//
      switch (fork())
      {
            case -1:   // No pudo crear el proceso
                  printf ("Error. No se crea proceso hijo");
                  break;


            case 0:   //se crea proceso hijo B
                  printf ("A crea el proceso A\n");
                  printf ("Soy proceso A, Mi pid es %d \n", getpid());
			//se lee de la tuberia
			while(1);
			if(a==1){                   
			close(ipc_ac[1]);
      			leido = read(ipc_ac[0], buff2, sizeof(buff2));
     			if(leido < 1){
            		write (0, "\nError al leer tuberia\n", sizeof("\nError al leer tuberia\n"));
      			}else {                 
			     // Escribir en consola
			     write (0, "\nLeido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
			     write (0, buff2, leido);
			     write (0, "\n", sizeof("\n")); 
				}
                  
                break;
      }
wait(NULL);
 mq_unlink(MQ_PATH);
//--creo la cola de mensajes--//
      // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff1);
   attr.mq_maxmsg = 5;
   
   // Se crea cola de mensajes
   descriptor_mq = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0777, &attr); 
   if (descriptor_mq < 0) {
      printf ("error en mq_open()");   
      exit(-1); }
      
   printf("Cola de mensajes creada\n");

	
		
			
//exit(0);
  }
}
//------------------------------------------------------------//


