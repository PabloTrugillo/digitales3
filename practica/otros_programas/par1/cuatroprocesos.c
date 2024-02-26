#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h> 

int t;
int a[5]={0,0,0,0,0};
int main (){
	for(t=0; t < 2 ; t++){
	a[t]= fork();
	//línea en blanco
	}
	if ((a[0]==0) && (a[1]>0)) {
	kill(a[1],SIGKILL);//
	exit(0);
	}
	if (a[0]==0) {
	while(1);
	}
	if (a[1]==0) {
	//línea en blanco
	while(1);
	}
	kill(a[1],SIGKILL);//
   printf ("Mi pid es %d\n", getpid());
  sleep(2);
 exit(0);
}
