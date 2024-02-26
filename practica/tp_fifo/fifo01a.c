/*
 * Ejercicio 1 del TP FIFO  Tipica implementacion de una FIFO.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>// en esta libreria esta la fncion para crear la fifo
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/wait.h> 

#define MENSAJE "HOLA PROCESO HIJO"//aca definimos un mensaje
#define FIFO_PATH "/tmp/MI_FIFO"//aca tenemos definido el nombre de una fifo con su ruta 

int main(){

   int err, fifo_d;
   char buff[80];
   int leido;

   //unlink(FIFO_PATH);    //si la FIFO existe la borro
//al crear la fifo con mkfifo el primer argumento es el nombre y el otro los permisos 
   // FIFO puede ser leida, escrita y ejecutada por:
   err = mkfifo(FIFO_PATH, S_IRUSR|S_IWUSR|S_IXUSR|  // el usuario que la creo.
                           S_IRGRP|S_IWGRP|S_IXGRP|  // el grupo al que pertenece el usuario.
                           S_IROTH|S_IWOTH|S_IXOTH); // el resto de los usuarios del sistema.

   //err = mkfifo(FIFO_PATH, 0777);// el numero en octal representa los permisos 
   if(err == -1) {//analiza q le devuelve la mkfifo
      write (STDOUT_FILENO, "\nError al crear FIFO, la FIFO ya existe", sizeof("\nError al crear FIFO, la FIFO ya existe"));
   }else {
      write (STDOUT_FILENO, "\nFIFO creado correctamente\n", sizeof("\nFIFO creado correctamente\n"));
   }

   switch (fork()){//crea un proceso hijo

      case -1:   //ERROR
         write (STDOUT_FILENO, "\nError al crear hijo", sizeof("\nError al crear hijo"));
         return -1;
      break;

      case 0:      //Hijo
         write (STDOUT_FILENO, "\nEntrando proceso HIJO", sizeof("\nEntrando proceso HIJO"));

         sleep(5);

         fifo_d = open(FIFO_PATH, O_RDWR, 0); // O_NONBLOCK // abre la fifo con permiso de solo lectura
         if(fifo_d == -1){
            write (STDOUT_FILENO, "\nHIJO: Error al abrir FIFO ", sizeof("\nHIJO: Error al abrir FIFO "));
            return -1;
         }else {
            write (STDOUT_FILENO, "\nHIJO: FIFO abierto correctamente", sizeof("\nHIJO: FIFO abierto correctamente"));
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
      
      default:     //Padre
         write (0, "\nEntrando proceso PADRE", sizeof("\nEntrando proceso PADRE")); 
      
         sleep(2);

         fifo_d = open(FIFO_PATH, O_WRONLY | O_NONBLOCK, 0);// el proc padre abre la fifo con permiso de solo escritura
         if(fifo_d == -1){
            write(STDOUT_FILENO, "\nPADRE: Error al abrir FIFO ", sizeof("\nPADRE: Error al abrir FIFO "));
            return -1;
         }else {
            write(STDOUT_FILENO, "\nPADRE: FIFO abierto correctamente", sizeof("\nPADRE: FIFO abierto correctamente"));
         }

         // Se escribe en el FIFO
         err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
         if(err == -1) {
            write(STDOUT_FILENO, "\nPADRE: Error al escribir en FIFO", sizeof("\nPADRE: Error al escribir en FIFO"));
         } else {
            write(STDOUT_FILENO, "\nPADRE: Escritos MENSAJE en FIFO", sizeof("\nPADRE: Escritos MENSAJE en FIFO"));
         }
         
         close(fifo_d);
         
         wait(NULL);   

         write (0, "\nSaliendo proceso PADRE\n", sizeof("\nSaliendo proceso PADRE\n")); 
               
      break;   
   }
   
   //Eliminación FIFO
   if (unlink(FIFO_PATH) < 0){
      printf("\nNo se puede borrar FIFO.\n"); }
      
   exit(0);

}
