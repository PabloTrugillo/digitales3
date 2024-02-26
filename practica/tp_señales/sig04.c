 /* Ejercicios 3 y 4 de TP Seniales
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
int flag;

void sig_user1(int a){
   write(STDOUT_FILENO,"Me rehuso a terminar\n", sizeof("Me rehuso a terminar\n"));
}

int main ()
{

   //signal(SIGKILL, sig_user1);
   signal(SIGUSR1, sig_user1);  //Ctrl + z
   
   printf("soy el proceso PID = %d\n", getpid());   
   
   while(1);
   
   exit(0);
}
/* en el punto 4 no puede ignorar la señal sigkill y al ejecutarla en otra consola el proceso termina.En el  punto 5 la señal sigurs devuelve un entero e imprime por salida standard el texto. Con control+C podemos finalizar el proceso*/
