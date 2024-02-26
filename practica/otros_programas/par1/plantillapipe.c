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

   if (pipe(ipc) == -1)//crea la pipe, le pone el nombr entre parentesis y pregunt si da error
      exit(-1);// si no se puede crear la tuberia , el programa termina
	//ALTERNATIVA: pipe(ipc); o pipe(a);

   switch (fork()){ // aplico el fork y analizo el valor numerico que devuelve
      //ALTERNATIVA: a=fork(); o  b=fork();
      case 0:// estamos situeados en el proceso hijo
	//ALTERNATIVA: if(b == 0){  }
         //close(ipc[1]); cierra el descritor de escritura del proceso hijo antes de escribir en la tuberia, ESTO ES UN ERROR
		close(ipc[0]);//cierra descriptor de lectura del proc hijo porque tiene que escribir y NO leer
         strncpy(buff, DATA, sizeof(DATA)); // copiar el el dato definido arriba al buffer  con su tamaño 
         write(ipc[1], buff, sizeof(DATA)); // escribe el contenido de buff en la tuberia 
         exit(0); // termina el proceso 
         
      default:// aca estoy en el proceso padre
       		 close(ipc[1]);//cierra descriptor de escrit del proc padr xq tiene q leer y NO escribir
         leido = read(ipc[0], buff, sizeof(buff));//leeemos del descrip de salida del proceso padre y guardamos en buff
		//ALTERNATIVA: "leido" puede ser reemplazado por una letra de variable entera declarada arriba
            write (STDOUT_FILENO, buff, leido-1);// imprime la salida del buffer por la salida standar q es la pantalla de consola
            
         }
         wait(NULL);//elspera q termine el hijo

         exit(0);//termina
   }
}
