/*
 * Ejercicio 2 de TP PIPE
 * 
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h> 

#define DATA "INFORMACION IMPORTANTE"// defino el el dato que va a ser una linea de caracteres
#define BUFF_SIZE 80// defino el tamaño del buffer

int main (){

   int ipc[2], proc;//
   int leido, success;
   char buff[BUFF_SIZE] = {0};
   int prueba;
   if (pipe(ipc) == -1)
      exit(-1);// si no se puede crear la tuberia , el programa termina

   switch (fork()){ // aplico el fork y analizo el valor numerico que devuelve
      
      case 0:// estamos situeados en el proceso hijo
         //close(ipc[1]); //cierra el descritor de escritura del proceso hijo antes de escribir en la tuberia, este es el error
         strncpy(buff, DATA, sizeof(DATA)); // copiar el el dato definido arriba al buffer  con su tamaño 
         prueba = write(ipc[1], buff, sizeof(DATA)); // escribe el contenido de buff en la tuberia
        // if(prueba < 1){
         printf("prueba = %d\n", prueba);
         //}
         printf("soy proc hijo... \n");
        // write (STDOUT_FILENO, buff, sizeof(DATA));
         exit(0); // termina el proceso 
         
      default:// aca estoy en el proceso padre
         printf("Leyendo tuberia... \n");
         leido = read(ipc[0], buff, sizeof(buff));//leeemos del descrip de salida del proceso padre y guardamos en buff
         if(leido < 1){// si no lo puede leer, imprime en pantalla la linea de abajo
            write (STDOUT_FILENO, "\nError al leer tuberia", sizeof("\nError al leer tuberia"));
         }else {// si lo lee hace lo sigte

            write (STDOUT_FILENO, "Leido de la tuberia ", sizeof("\nLeido de la tuberia"));
            write (STDOUT_FILENO, buff, leido-1);// imprime la salida del buffer por la salida standar q es la pantalla de consola
            printf(", por el proceso padre, pid %d \n", getpid());// imprime pid de proc padre
         }
         wait(NULL);//elspera q termine el hijo

         exit(0);//termina
   }
}
