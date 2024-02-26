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

#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"
int leido;
char buff[100];
int main(){

int ipc1[2],ipc2[2];

pipe(ipc1);
pipe(ipc2);

switch(fork()){
case 0:
		

		switch(fork()){// Nieto de C e hijo de B//PROCESO A 
		case 0:	
			printf("\nSoy el Proceso A, voy a escribir en tubería 1\n");
			close(ipc1[0]);		
			strncpy(buff, MENSAJE_A, sizeof(MENSAJE_A)); 
			write(ipc1[1], buff, sizeof(MENSAJE_A));
			exit(0);
			break;
		default:  //hijo de C
			//wait(NULL);//no hace falta xq pipe es bloqueante
			
			close(ipc1[1]);
			leido = read(ipc1[0], buff, sizeof(buff));
			if(leido < 1){
			write (0, "\nError al leer tuberia1", sizeof("\nError al leer tuberia"));
			}else {
			printf("\nSoy el Proceso B, hijo de C y voy a leer de tubería 1\n");
			write (0, "Leido de la tuberia ", sizeof("\nLeido de la tuberia"));
			write (0, buff, leido-1);
			printf(", por el proceso padre, pid %d \n", getpid());
				}
			printf("\nSoy el Proceso B, hijo de C y voy a escribir en tubería 2\n");
			close(ipc2[0]);
			strncpy(buff, MENSAJE_B, sizeof(MENSAJE_B)); 
			write(ipc2[1], buff, sizeof(MENSAJE_B));
			exit(0);	
			}
			break;
//exit(0);
break;
default:

		
		//wait(NULL);
		
		close(ipc2[1]);
		leido = read(ipc2[0], buff, sizeof(buff));
		if(leido < 1){
		write (0, "\nError al leer tuberia2", sizeof("\nError al leer tuberia"));
		}else {
			printf("\nSoy el Proceso C, Padre de B y voy a leer de tubería 2\n");
			write (0, "Leido de la tuberia ", sizeof("\nLeido de la tuberia"));
			write (0, buff, leido-1);
			printf(", por el proceso padre, pid %d \n", getpid());
			}
break;
}
}
