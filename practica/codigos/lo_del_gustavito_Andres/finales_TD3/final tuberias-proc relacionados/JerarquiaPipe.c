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

#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"

int main (){
int ipc1[2],ipc2[2];
int leido;
char buff[80];

pipe(ipc2);
switch(fork()){
case 0://PROCESO B
pipe(ipc1);
	switch(fork()){
	case 0://PROCESO A
	close(ipc1[0]);
	// Se escribe en la tuberia
	write(ipc1[1], MENSAJE1, sizeof(MENSAJE1));
	exit(0);
	break;
	default:
	//wait(NULL);// No hace falta xq pipe ya es bloqueante
	close(ipc1[1]);
	leido = read(ipc1[0], buff, sizeof(buff));
	if(leido < 1){
	write (0, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
			}else {
			
			// Escribir en consola
			write (0, "\nLeido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
			write (0, buff, leido-1);
				}
close(ipc2[0]);
write(ipc2[1], MENSAJE2, sizeof(MENSAJE2));
exit(0);
	}
break;

default:
//wait(NULL);
close(ipc2[1]);
leido = read(ipc2[0], buff, sizeof(buff));
	if(leido < 1){
	write (0, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
			}else {
			
			// Escribir en consola
			write (0, "\nLeido de la tuberia: ", sizeof("\nLeido de la tuberia: "));
			write (0, buff, leido-1);
			write(0,"\n",sizeof("\n"));	

}}
sleep(2);
exit(0);
}


