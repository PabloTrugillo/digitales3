/*
 * Ejercicio 2 de TP Procesos
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid

int main (){

	pid_t pid;
	int i;

	pid = fork();
	switch(pid)
	{
	case -1: 
		printf("error");
		return -1;
		break;		
	case 0:
		printf("\n yo soy el proceso hijo, mi pid es %d, el pid de proceso padre es %d y fork devolvio %d\n\n",getpid(), getppid(), pid);
		
		sleep(30);
		break;
	default:
	printf ("\n soy el proceso padre mi pid es %d y el pid de papa es %d. fork() devolvio %d\n", getpid(), getppid(), pid);
	//sleep(15);
	break;
	}

	exit(0);
}
