#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid

int t;
int a[5]={0,0,0,0,0};

int main (){
	for(t=0;t<3;t++){
		//printf(" %d \n",t);
		a[t]=fork();
		//printf ("Mi pid es %d y el pid de papa es %d. fork() devolvio %d y el valor de la variable i es %d\n", getpid(), getppid(), a[t],t);
	}
	if(a[1] == 0){		
		printf ("1-Mi pid es %d hijo de %d\n", getpid(),getppid());
		exit(0);
	}
	a[4]=fork();
	if(a[0] == 0){		
		printf ("2-Mi pid es %d hijo de %d\n", getpid(),getppid());
		exit(0);
	}	
	if(a[3] == 0){		
		printf ("3-Mi pid es %d hijo de %d\n", getpid(),getppid());
		sleep(2);		
		exit(0);
	} 
	printf ("4-Mi pid es %d hijo de %d\n", getpid(),getppid());
	exit(0);

}
