/*
Tres procesos NO relacionados (A, B y C) se comunican una fifo.
1) El proceso A crea la fifo , y se queda en una espera activa.
2) El proceso B esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la fifo "MENSAJE" y vuelve a la espera activa.
3) El proceso C esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la fifo "ABCDEFGHIJ" y vuelve a la espera activa.
4) El proceso A lee la fifo cada vez que escribe B o C y recibe la señal SIGINT y muesta lo leído.

*/

//Compile el programa: gcc -o fifotrespB fifotrespB.c -lpthread -lrt
//Ejecute el programa ./fifotrespB

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

#define FIFO_PATH1 "/tmp/MI_FIFO"
#define MENSAJE "ENVIADO INFORMACION IMPORTANTE"

int fifo_d;
int err;
int leido;
char buff[100], buff1[100], buff2[100];


//// Manejador ///////////////////////////////////////////////////////////////////////////////////
void manj1(int a){
        printf("señal recibida\n");
        fifo_d = open(FIFO_PATH1, O_RDWR, O_NONBLOCK,0);  //Abre y devuelve descriptor
	/*err=write(fifo_d, MENSAJE, sizeof(MENSAJE));
	write(fifo_d, buff, sizeof(buff));
	if(err== -1){
	printf("error");
	    return -1;}
	if(err==0){
	write(STDOUT_FILENO,"Mensaje enviado\n", sizeof("Mensaje enviado\n"));
	exit(0);}
	//exit(0);*/
	err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
	write(fifo_d, buff, sizeof(buff));
	if(err == -1){
	write(STDOUT_FILENO, "\n error al escribir fifo \n", sizeof("\n error al escribir fifo \n"));
	return -1;
	}else{//si logra escribir, muestra lo escrito en pantalla
	write(STDOUT_FILENO, "\n escrito en fifo: \n", sizeof("\n escrito en fifo: \n"));
	write(STDOUT_FILENO, MENSAJE, sizeof(MENSAJE));
	printf("\n");
	exit(0);}
	}
//main//////////////////////////////////////
int main(){
		signal(SIGINT,manj1);
		//unlink(FIFO_PATH1); 					//La borro si ya exite
		//mkfifo(FIFO_PATH1, 0777);				//Crea la FIFO1
		
		printf("PID proceso B = %d\n", getpid());
		
		
		 while(1);
	exit(0);
}
