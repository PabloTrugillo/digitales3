/*
Tres procesos (A, B y C) NO relacionados se comunican de la siguente forma.
1) El proceso A crea un socket stream servidor y queda a la espera de una conexión
2) El proceso A (servidor) crea un hilo cuando se conecta un cliente
3) El proceso B (cliente1) crea un socket stream cliente se conecta con el servidor
4) El proceso C (cliente2) crea un socket stream cliente se conecta con el servidor
5) El proceso A (servidor)cuando recibe algo en por el socket lo lee y muestra en pantalla
6) El proceso B (cliente1) cuando recibe la señal SIGINT le envía al servidor el mensaje  "0123456789"
7) El proceso C (cliente2) cuando recibe la señal SIGINT le envía al servidor el mensaje  "ABCDEFGHIJ"
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



#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"

/*
//------------------------------------------------------------//
void manejador_senial(int a){
   
}
//------------------------------------------------------------//
*/

/*
//------------------------------------------------------------//
void HILOA(void) {   


   pthread_exit(0);
}
//------------------------------------------------------------//
*/

/*
//------------------------------------------------------------//
void HILOB(void) {   


   pthread_exit(0);
}
//------------------------------------------------------------//
*/


//------------------------------------------------------------//
int main() {

   
   pthread_exit(0);
}

//------------------------------------------------------------//

