/*
 * Ejercicios 3 y 4 de TP Seniales
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

/*
void manejador_senial(int a){
   
}
*/
int main ()
{

   //signal(SIGKILL, SIG_IGN);
   signal(SIGTSTP, SIG_IGN);  //Ctrl + z
   
   printf("Proceso PID = %d\n", getpid());   
   
   while(1);
   
   exit(0);
}

/*en el punto 2 del practico, se ignora la señal control+C la señal SIGKILL no es ignorada,
 al ejecutarla en otracosola, el 
proceso termina
 en el punto 3 si ignora la señal control+Z, es decir no podemos detener el proceso. 
La señal control+c ahora NO es ignorada*/

