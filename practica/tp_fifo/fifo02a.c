/*
 * Ejercicio 2 del TP FIFO
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/wait.h> 

#define MENSAJE "HOLA PROCESO HIJO"
#define FIFO_PATH "/tmp/MI_FIFO"

int main(){

   int err, fifo_d;
   char buff[80];
   int leido;
   
   unlink(FIFO_PATH);    //si la FIFO existe la borro                   

   // FIFO puede ser leida, escrita y ejecutada por: 
   err = mkfifo(FIFO_PATH, 0777); // el resto de los usuarios del sistema.
   if(err == -1) {
      write(STDOUT_FILENO, "\nError al crear FIFO, la FIFO ya existe", sizeof("\nError al crear FIFO, la FIFO ya existe"));
   }else {
      write(STDOUT_FILENO, "\nFIFO creado correctamente\n", sizeof("\nFIFO creado correctamente\n"));
   }

   switch (fork()){

      case -1:
         write(STDOUT_FILENO, "\nError al crear hijo", sizeof("\nError al crear hijo"));
         return -1;
      break;

      case 0:

         write(STDOUT_FILENO, "\nEntrando proceso HIJO", sizeof("\nEntrando proceso HIJO"));

         sleep(5);

         fifo_d = open(FIFO_PATH, O_RDONLY, 0); 
         if(fifo_d == -1){
            write(STDOUT_FILENO, "\nHIJO: Error al abrir FIFO ", sizeof("\nHIJO: Error al abrir FIFO "));
            return -1;
         }else {
            write(STDOUT_FILENO, "\nHIJO: FIFO abierto correctamente", sizeof("\nHIJO: FIFO abierto correctamente"));
         }

         // Se lee FIFO
         leido = read(fifo_d, buff, sizeof(buff));   
         if(leido == -1){
            write(STDOUT_FILENO, "\nHIJO: Error al leer en FIFO", sizeof("\nHIJO: Error al leer en FIFO"));
         }else {
            write(STDOUT_FILENO, "\nHIJO: Leido del FIFO: ", sizeof("\nHIJO: Leido del FIFO: "));
            write(STDOUT_FILENO, buff, leido-1);
            write(STDOUT_FILENO, "\n", sizeof("\n"));
         }

         close(fifo_d);

         write(STDOUT_FILENO, "\nSaliendo proceso HIJO\n", sizeof("\nSaliendo proceso HIJO\n"));
         exit(0);
      break;

      default:
         write(STDOUT_FILENO, "\nEntrando proceso PADRE", sizeof("\nEntrando proceso PADRE")); 

         /*  
         //El padre que debe hacer con la FIFO ?
         */      
         fifo_d = open(FIFO_PATH, O_WRONLY, 0); 
         if(fifo_d == -1){
            write(STDOUT_FILENO, "\npadre: Error al abrir FIFO \n", sizeof("\npadre: Error al abrir FIFO \n"));
            return -1;
         }else {
            write(STDOUT_FILENO, "\npadre: FIFO abierto correctamente\n", sizeof("\npadre: FIFO abierto correctamente\n"));
		err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
         if(err == -1) {
            write(STDOUT_FILENO, "\nPADRE: Error al escribir en FIFO", sizeof("\nPADRE: Error al escribir en FIFO"));
         	} else {
            write(STDOUT_FILENO, "\nPADRE: Escritos MENSAJE en FIFO", sizeof("\nPADRE: Escritos MENSAJE en FIFO"));
         		}
         
         close(fifo_d);
         }
         wait(NULL);   
         write (0, "\nSaliendo proceso PADRE\n", sizeof("\nSaliendo proceso PADRE\n")); 
         
         break;
   }
   
   //Eliminación FIFO
   //if (unlink(FIFO_PATH) < 0) {
    //   printf("\nNo se puede borrar FIFO.\n"); }
       
   exit(0);   
}
