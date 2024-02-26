/*Escriba un programa seg05.c que realice las siguientes tareas:
a. Cree un proceso hijo que haga una espera activa de dos segundos y finalice con la
función exit(0).
b. Luego de crear el proceso hijo, el padre debe ejecutar una espera activa por más de
10 segundos. Luego de la finalización del proceso hijo, verifique con pstree si este
está en estado zombie.
c. Establezca en el proceso padre un manejador para la señal SIGCHLD. En el
manejador de esta señal se debe leer el estado de finalización del hijo con la función
wait(&status). Además se debe mostrar lo devuelto por la función wait() y el valor
de status. ¿En este caso, el proceso hijo no queda en estado zombie?*/


/*
 * Ejercicio 8 de TP Seniales
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int leido, b;
pid_t pid1, pid2=0;

void sigchild_hdl(int a){
	pid2=wait(&leido);
			}
int main (){
	signal(SIGCHLD, sigchild_hdl);
	b=0;
	pid1 = fork();
	if(pid1==0){ // hijo
		printf("proceso hijo. PID = %d\n", getpid());
		sleep(2);
		exit(0);
		   }
	while(pid2==0){
		      
	pid2=wait(&leido);
	printf("proceso hijo, devuelve: %d, %d\n", pid2, leido);
	printf("proceso padre finalizada, PID = %d\n", getpid());
	exit(0);
	  }	
}

void sig_user1(int a){
   
	write(STDOUT_FILENO,"\n Me reuso a terminar\n",sizeof("\n Me reuso a terminar\n"));
/*}

int main (){

   signal(SIGKILL, sig_user1);
   signal(SIGUSR1, sig_user1);  //Ctrl + z
   
   printf("Soy el Proceso PID = %d\n", getpid());   
   
   while(1);*/
   
   exit(0);
}


