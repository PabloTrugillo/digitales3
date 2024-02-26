 /* Ejercicios 6 de TP Seniales
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

//aca esta lo que va a hacer el manejador de señal
void sigchild_hdl(int a){
   write(STDOUT_FILENO,"termino hijo\n", sizeof("termino hijo\n"));
}
int main (){
	pid_t pid1, pid2, pid3;
	signal(SIGCHLD,sigchild_hdl);//manejador de señal
	pid1 = fork();
	if (pid1 == 0){
	   printf("proceso hijo 1. PID = %d\n",getpid());
	   while(1);
	}
	else{
		pid2 = fork();
		if (pid2 == 0){
	   	printf("proceso hijo 2. PID = %d\n",getpid());
	   	while(1);
		}
		else{
			pid3 = fork();
			if (pid3 == 0){
		   	printf("proceso hijo 3. PID = %d\n",getpid());
		   	while(1);
			}
			else{
			//espera activa del proceso padre
			sleep(2);
			kill(pid1,SIGKILL);
			sleep(2);
			kill(pid2,SIGKILL);
			sleep(2);
			kill(pid3,SIGKILL);
			}
		}
	}
 
 sleep(2);
 exit(0);
}




