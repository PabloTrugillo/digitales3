/*
 * PARCIAL 1: programa para ser ejecutado en el proceso padre.
 * 
 * Comando para compilar:
 * 
 * 	gcc -m32 -c padre.c -lrt && gcc -m32 hijo.o padre.o -o padre -lrt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

int hijo (mqd_t mqd);

#define MQ_PATH "/MQ_TD3"  // El nombre de la cola de mensajes debe empezar con "/"

int main(void) 
{
	int pid;  	 	
	int err, leido;
	char buff[1024];   
	mqd_t mqd; 
	struct mq_attr attr;
	struct mq_attr attr_rcv;

	// Se elimina cola de mensajes
	err = mq_unlink(MQ_PATH);
	if(err == -1){
		printf ("Padre: error en mq_unlink (%d).\n", err);
		//~ exit(-1);   	
	}
	else{
		printf("Padre: Cola de mensajes eliminada (%d)\n", err);
	}
			
	// Se fijan algunos parametros de la cola de mensajes antes de crearla
	attr.mq_msgsize = sizeof(buff);
	attr.mq_maxmsg = 3;
	
	// Se abre o crea cola de mensajes
	mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0777, &attr); 
	if (mqd < 0) {
		printf ("Padre: error en mq_open (%d).\n", mqd);    
		exit(-1); 
	}
	else{
		printf("Padre: cola de mensajes creada.\n"); 
	}
	
	if ((pid = fork()) == -1)
	{
		perror("Padre: fork");
		return -1;
	}

	// codigo que ejecuta el hijo
	if (0 == pid) 
	{ 
		hijo(mqd);	
		return 0;
	}
	// codigo que ejecuta el padre   
	else
	{
		printf("Padre en ejecucion... \n"); 

		sleep(1);

		// **** RESOLUCION DEL EJERCICIO *******************************
		mq_getattr(mqd, &attr_rcv);
		
		mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
		
		printf("Mensaje leido: %s\n", buff);
	
		// *************************************************************
		
		wait(NULL);
		return 0;
	}
}
