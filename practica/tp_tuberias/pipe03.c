/*
 * Ejercicio 3 de TP PIPE
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h> 

#define FRASE_A "INFORMACION IMPORTANTE"
#define FRASE_B "INFORMACION IMPORTANTE"
#define BUFF_SIZE 80

void pipe_sign_handler(int a){// funcion a ejecutar si llega la señal sigpipe
   
   write (STDOUT_FILENO, "\n Problema con pipeline.\n ", sizeof("\n Problema con pipeline.\n"));
}

int main (){

   int ipc[2], proc, ipc2[2];
   int leido, success, leido2;
   char buff[BUFF_SIZE] = {0};

   signal(SIGPIPE, pipe_sign_handler);//manejador de señal
   
   pipe(ipc);//crea la tuberia 

   switch (fork()){ //hacemos un fork y analizamos el valor q devuelve
      
      case 0://estamos en el proc hijo
      close(ipc[0]);//cierro descriptor de salida del hijo      
      strncpy(buff, FRASE_A, sizeof(FRASE_A)); //lo guardado en frase_A lo guardo en el bufer
      write(ipc[1], buff, sizeof(FRASE_A));//escribe en la tuberia y lo guarda en el bufer
      exit(0);//termina
      break;//salir
      
      default:// ahora estoy en el padre
	pipe(ipc2);//creamos una segunda tuberia
      switch (fork()){ //vuelve a aplicar un fork al proc padre y analiza su valor 
            
         case 0:// estamos en el nuevo proc hijo
            close(ipc2[0]);//cerramos descriptor de salida del nuevo hijo               
            strncpy(buff, FRASE_B, sizeof(FRASE_B)); //lo guardado en frase_B lo guardo en el bufer
            write(ipc2[1], buff,    sizeof(FRASE_B));//escribe en la tuberia nueva y lo guarda en el bufe
            exit(0);      
         break;
         default://aca estamos nuevamente en el proceso padre
            close(ipc[1]);//cierra descriptor de entrada de proceso padre en la tuberia
	    close(ipc2[1]);//cierra descriptor de entrada de proceso padre en la segunda tuberia
           // int i;//declara la variable entera i
           // for(i=0; i<2; i++){//abro lazo for para hacer bucle doble
               leido = read(ipc[0], buff, sizeof(buff));//el padre lee de la salida de la tuberia y lo leido lo guarda en buff
	leido2 = read(ipc2[0], buff, sizeof(buff));//el padre lee de la salida de la seg tuberia y lo leido lo guarda en buff
               if(leido < 1){//si no puede leer imprime error
                  write (STDOUT_FILENO, "\nError al leer 1ra tuberia", sizeof("\nError al leer 1ra tuberia"));
		}else {//si pudo leer ejecuta lo sigte
                  write (STDOUT_FILENO, "\nLeido de la primer tuberia\n ", sizeof("\nLeido de la primer tuberia\n"));
                  write (STDOUT_FILENO, buff, leido-1);//escribe el contemnido del buf en pantall
                  printf(", por el proceso padre, pid %d \n", getpid());//imprime el pid del proc padre
               }
		if(leido2 < 1){//si no puede leer la segunda tuberia imprime error
                  write (STDOUT_FILENO, "\nError al leer 2da tuberia\n", sizeof("\nError al leer 2da tuberia\n"));
               }else {//si pudo leer ejecuta lo sigte
                  write (STDOUT_FILENO, "\nLeido de la2 tuberia\n ", sizeof("\nLeido de la2 tuberia\n"));
                  write (STDOUT_FILENO, buff, leido2-1);//escribe el contemnido del buf en pantall
                  printf(", por el proceso padre, pid %d \n", getpid());//imprime el pid del proc padre
               }
           // }//cierro el for

            close(ipc[0]);//cierro descriptor de salida del proc padre
	    close(ipc2[0]);//cierro descriptor de salida del proc padre en la segunda tuberia
            wait(NULL);// espera q terminen los dos hijos
            wait(NULL);
            exit(0);//termina el proceso padre
         break;
      }
   }   
}

//el problema que tiene es que el padre no puede identificar que parte escribio el proceso A y el B
