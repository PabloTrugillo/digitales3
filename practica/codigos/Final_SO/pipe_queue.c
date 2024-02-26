//Compile el programa: gcc -o pipe_queue pipe_queue.c -lpthread -lrt
//Ejecute el programa ./pipe_queue
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

#define FRASE_A "0123456789"
#define FRASE_B "ABCDEFGHIJ"
#define MQ_PATH "/Rodol"
#define BUFF_SIZE 80

int ipc1[2], ipc2[2];
int leido,aux1=0,aux2=0;
char buff[BUFF_SIZE] = {0};
struct mq_attr attr, attr_rcv;


void pipe1()
{
    strncpy(buff, FRASE_A, sizeof(FRASE_A)); 
    write(ipc1[1], buff, sizeof(FRASE_A));
	aux1=1;
	exit(0);
	
}
void pipe2()
{
    strncpy(buff, FRASE_B, sizeof(FRASE_B)); 
    write(ipc2[1], buff,    sizeof(FRASE_B));
	aux2=1;
    exit(0);
	
}

int main (int argc , char const * argv[]){

  
   signal(SIGUSR1,pipe1);
   signal(SIGUSR2,pipe2);
	
   /////////////////CREAMOS LA QUEUE///////////////
   int fd_queue;
   attr.mq_msgsize = 8192; ///ESTO VA SIEMPRE ANTES DE CREAR LA COLA 
   attr.mq_maxmsg = 2;
   fd_queue = mq_open(MQ_PATH,O_WRONLY | O_CREAT, 0777, &attr);	
  
   ////////////////////////////////////////////////
	
	
   pipe(ipc1);
   pipe(ipc2);
   switch (fork()){ 
      
      case 0:
         close(ipc1[0]);  
	     printf("Proceso B PID: %d mi padre: %d\n",getpid(),getppid());
         while(1){}
      break;
      
      default:
      switch (fork()){ 
            
         case 0:
            close(ipc2[0]);   
			printf("Proceso C PID: %d mi padre: %d\n",getpid(),getppid());
            while(1){}
         break;
         default:
            close(ipc1[1]);
			close(ipc2[1]);
			wait(NULL);
            wait(NULL);
            int i;
          	
				
				  leido = read(ipc1[0], buff, sizeof(buff));
				  //Escribir por consola//
                  write (1,"\nLeido del Pipe 1: ", sizeof("\nLeido del Pipe 1:"));
                  write (1, buff, leido-1);
                  printf("\nPor el proceso padre PID: %d\n", getpid());
				  
				  //Escribimos en la QUEUE
			      mq_send(fd_queue, buff, strlen(buff)+1,0);
			  
	
				  leido = read(ipc2[0], buff, sizeof(buff));
				  //Escribir por consola//
                  write (1,"\nLeido del pipe 2: ", sizeof("\nLeido del pipe 2:"));
                  write (1, buff, leido-1);
                  printf("\nPor el proceso padre, PID: %d \n", getpid());
					
			      //Escribimos en la QUEUE
			      mq_send(fd_queue, buff, strlen(buff)+1,0);
				   
				
		
            close(ipc1[0]);
			close(ipc2[0]);
			mq_close(fd_queue);
            exit(0);
         break;
      }
   }   
}
