#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main ()
{
   sleep(5);//el programa se suspende durante 5 seg antes de continuar 
   
   
   exit(5); //con el echo en consola devuelve el 5
}
