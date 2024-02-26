#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    // Define pid_t
#include <unistd.h>       // Define fork, getpid y getppid
int a,b,c,d;

int main ()
{
        a = fork();
	b = fork();
	c = fork();
	d = 0; 
	printf ("1-Mi pid es %d\n",getpid());
	if (a==0) {
		exit(0);
        	
        }	
	printf ("2 - mi pid es %d\n", getpid());
                d = fork(); 
		if (b==0){
		exit(0);
		}
	
	/*printf ("3- mi pid es %d\n", getpid());
                        
	if (c==0) {
	exit(0);
	}	

         printf ("4 - mi pid: %d\n", getpid());
        
	    exit(0);*/
}
	
