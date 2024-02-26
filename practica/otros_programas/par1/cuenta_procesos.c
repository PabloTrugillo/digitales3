// INDIQUE CUANTOS PROCESOS SE EJECUTAN POR LINEA DE CODIGO 29/2003

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int a,b,c,d,e=0;

int main (){
	a = fork();//2
	b = fork();//4
	c = fork();//8
	if (a == 0) {
	exit(0);
	printf ("1 -mi pid es %d\n", getpid());
	}
	printf ("2 mi pid es %d\n",getpid());//4
	if (b == 0) {
	exit(0);
	}
	d = fork();
	printf ("3 mi pid es %d\n", getpid());//4
	if (c > 0) {
	exit(0);
	}
	printf ("4 mi pid es %d\n",getpid());//2
	//sleep(40);
	exit(0);
	}

//RTA: 


