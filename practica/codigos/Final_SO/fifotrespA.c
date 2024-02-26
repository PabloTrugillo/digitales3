/*
Tres procesos NO relacionados (A, B y C) se comunican dos fifos.
1) El proceso A crea las fifos , y se queda en una espera activa.
2) El proceso B esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la fifo1 "MENSAJE" y vuelve a la espera activa.
3) El proceso C esta en una espera activa y cada vez que recibe la señal SIGINT escribe en la fifo2 "ABCDEFGHIJ" y vuelve a la espera activa.
4) El proceso A lee la fifo cada vez que escribe B o C y recibe la señal SIGINT y muesta lo leído.

*/

//Compile el programa: gcc -o fifotrespA fifotrespA.c -lpthread -lrt
//Ejecute el programa ./fifotrespA

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
int fifo_d;
int err;
int leido1;
char buff[100], buff1[100], buff2[100];
void manj1(int a){
        printf("señal recibida\n");
         
        leido1 = read(fifo_d, buff, sizeof(buff));   //Devuelve la cantidad de bytes leidos de fifo
	
        write (1, "Leido de la FIFO: ", sizeof("Leido de la FIFO: "));
        write (1, buff, leido1-1); //ESCRIBO BUFF POR PANTALLA	
	write(STDOUT_FILENO, "\n", sizeof("\n"));
exit(0);
}

int main(){
        signal(SIGINT,manj1);
  printf("proceso A = %d\n", getpid());
//// Crear Fifo /////////////////////////////////////////////////////////////////////////////////////
	unlink(FIFO_PATH1); 					//La borro si ya existe
	mkfifo(FIFO_PATH1, 0777);				//Crea la FIFO1
	
	fifo_d = open(FIFO_PATH1, O_RDWR,O_NONBLOCK, 0);  //Abre y devuelve descriptor       
	
	 while(1);
	exit(0);
}

