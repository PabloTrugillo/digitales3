/*
 * Ejercicio 4a del TP FIFO
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/wait.h> 

#define MENSAJE "HOLA PROCESO A"
#define FIFO_PATH "/tmp/MI_FIFO"

int main(){

   int err, fifo_d;
   char buff[80];
   int leido;
	write(STDOUT_FILENO, "\n comienza el proceso B\n", sizeof("\n comienza el proceso B\n"));
	//err=mkfifo(FIFO_PATH, 0777);
	//if(err == -1){
	//write(STDOUT_FILENO, "\n error al crear fifo xq ya existe \n", sizeof("\n error al crear fi77fo xq ya existe \n"));
	//}else{
	//write(STDOUT_FILENO, "\n abriendo la fifo \n", sizeof("\n abriendo la fifo\n"));
	//}
	fifo_d = open(FIFO_PATH, O_RDONLY, 0);//abre la fifo con permisos de solo lectura
	if(fifo_d == -1){
	write(STDOUT_FILENO, "\n error al abrir fifo \n", sizeof("\n error al abrir fifo \n"));
	return -1;
	}else{//si la abre imprime lo sigte
	write(STDOUT_FILENO, "\nfifo abierta correctamente \n", sizeof("\nfifo abierta correctamente \n"));
	}
	//se lee en el fifo
	leido = read(fifo_d, buff, sizeof(buff));//lee la fifo q abrio y loq lee lo guarda en el buffer
	if(err == -1){
	write(STDOUT_FILENO, "\n error al leer fifo \n", sizeof("\n error al leer fifo \n"));
	return -1;
	}else{
	write(STDOUT_FILENO, "\n leido en fifo: \n", sizeof("\n leido en fifo: \n"));
	write(STDOUT_FILENO, buff, leido-1);//imprime en pantalla el contenido de buff con el tamaño de leido
	write(STDOUT_FILENO, "\n", sizeof("\n"));
	}
	
	close(fifo_d);//cierra la fifo
	unlink(FIFO_PATH);//elimina la fifo00
	write(STDOUT_FILENO, "\ntermina proceso B\n", sizeof("\ntermina proceso B\n"));
	exit(0);//aca termina el proc
}



