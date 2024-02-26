/*	MESA 01/06/2017
Tres procesos (A, B y C) se comunican por medio de una Cola de Mensaje.
1) El proceso C envía una señal (SIGUSR1) al proceso A. El proceso A escribe en la Cola de Mensaje "0123456789" y termina
2) El proceso C espera a que termine el procesos A, lee de la Cola de Mensaje y muestra lo leído.
3) El proceso C envía una señal (SIGUSR2) al proceso B. El proceso B escribe en la Cola de Mensaje "ABCDEFGHIJ" y termina.
4) El proceso C espera a que termine el procesos B, lee de la Cola de Mensaje, muestra lo leído y elimina la cola de mensajes.

Considerar los procesos con la siguiente jerarquía: Proceso C (padre), Proceso B (hijo1 de C), Proceso A (hijo2 de C)

*/

//Compile el programa: gcc -o name name.c -lpthread -lrt
//Ejecute el programa ./name


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

#define MENSAJEA "0123456789"
#define MENSAJEB "ABCDEFGHIJ"
#define MQ_PATH "/MQ_TD3"  // El nombre de la cola de mensajes debe empezar con "/"


int control1=0;
int control2=0;
//------------------------------------------------------------//
void manejador_senial1(int a){
   control1=1;
   write (1, "\nLlego SIGUSR1\n", sizeof("\nLlego SIGUSR1\n"));	
}
//------------------------------------------------------------//


void manejador_senial2(int b){
   control2=1;
   write (1, "\nLlego SIGUSR2\n", sizeof("\nLlego SIGUSR2\n"));	
}



//------------------------------------------------------------//
int main() {

   signal(SIGUSR1, manejador_senial1);
   signal(SIGUSR2, manejador_senial2);

   int err, leido;
   char buff[1024];   
   mqd_t mqd; 
   pid_t procesoB, procesoA;
   struct mq_attr attr, attr_rcv;
   
   
   // Se fijan algunos parametros de la cola de mensajes antes de crearla
   attr.mq_msgsize = sizeof(buff);
   attr.mq_maxmsg = 5;
   
   // Se crea cola de mensajes
   mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
   if (mqd < 0) {
      printf ("error en mq_open()");   
      return -1; }
      
   printf("Cola de mensajes creada\n"); 
   
   // Se leen parametros de la cola de mensajes
   if (mq_getattr(mqd, &attr_rcv) == -1){
      printf ("error en mq_getattr()");
      return -1; }
      
   printf("Nro max. de mensajes en cola de mensajes: %ld\n",attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n",attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n",attr_rcv.mq_curmsgs);

   procesoB = fork(); //crea hijo1 procesoB

   switch(procesoB){
        case -1: write(1, "\nERROR AL CREAR EL PROCESO B\n", sizeof("\nERROR AL CREAR EL PROCESO B\n"));
                 break;

	case 0: //hijo1 procesoB
		while(control2==0); //ESPERO LA SEÑAL SIGUSR2

		// Escribe en cola de mensajes
		 printf("\nsoy el proceso B (PID: %d), voy a escribir\n",getpid());
		 	
  		 err = mq_send(mqd, MENSAJEB, strlen(MENSAJEB)+1, 1);  //strlen nos da la longitud de una cadena
 		 if(err == -1){
      			printf ("error en mq_send()");
      			return -1;    }

   		printf("Mensaje enviado (%d)\n", err);
		exit(0);		
		break;

        default: //padre
		kill(procesoB, SIGUSR2); //envio al procesoB la señal SIGUSR2
		wait(NULL);		    //espero al procesoB

		// Se lee de cola de mensajes
   		leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);
   		if (( leido < 0 )){
     		 printf ("error en mq_receive()");
     		 return -1; }
		
		printf("Padre(PID:%d) lee. Mensaje leido: %s\n", getpid(), buff);

		procesoA = fork(); //crea hijo2 procesoA
		switch(procesoA){
     		
		case -1: write(1, "\nERROR AL CREAR EL PROCESO A\n", sizeof("\nERROR AL CREAR EL PROCESO A\n"));
                 	 break;

		case 0: //hijo2 ProcesoA
			while(control1==0); //ESPERO LA SEÑAL SIGUSR1

		// Escribe en cola de mensajes
		 	printf("\nsoy el proceso A (PID: %d), voy a escribir\n",getpid());
		 	
  		 	err = mq_send(mqd, MENSAJEA, strlen(MENSAJEA)+1, 1);  //strlen nos da la longitud de una cadena
 		 	if(err == -1){
      				printf ("error en mq_send()");
      				return -1;    }

   			printf("Mensaje enviado (%d)\n", err);
			exit(0);		
			break;
			
		default: //padre
			kill(procesoA, SIGUSR1); //envio al procesoA la señal SIGUSR1
			wait(NULL);		    //espero al procesoA

			// Se lee de cola de mensajes
   			leido = mq_receive(mqd, buff, attr.mq_msgsize, 0);
   			if (( leido < 0 )){
     				 printf ("error en mq_receive()");
     				 return -1; }
		
			printf("\nPadre(PID:%d) lee. Mensaje leido: %s\n", getpid(), buff);

			// Se cierra cola de mensajes
  			 err = mq_close(mqd);
   			if (( err < 0 )){
      				printf ("error en mq_close()");
      				return -1;   }

     			 printf("Cola de mensajes cerrada (%d)\n", err);
   
   			// Se elimina cola de mensajes
   			err = mq_unlink(MQ_PATH);
  			 if(err == -1){
     				 printf ("error en mq_unlink()");
      				return -1;   }

      			printf("Cola de mensajes eliminada (%d)\n", err);	 
					
			 
		}
   }  




exit(0);
}

//------------------------------------------------------------//

