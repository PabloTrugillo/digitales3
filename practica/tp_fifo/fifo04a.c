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

#define MENSAJE "HOLA PROCESO B"
#define FIFO_PATH "/tmp/MI_FIFO"//al nombre de la fifo le asigno su ruta

int main(){

   int err, fifo_d;
   char buff[80];
   int leido;
	write(STDOUT_FILENO, "\n comienza el proceso A\n", sizeof("\n comienza el proceso A\n"));
	err=mkfifo(FIFO_PATH, 0777);//creo la fifo pasandole el nombre y permisos
	if(err == -1){//si da error imprime lo sigte en pantalla
	write(STDOUT_FILENO, "\n error al crear fifo xq ya existe \n", sizeof("\n error al crear fifo xq ya existe \n"));
	}else{//si logra crearla imprime lo sigte en pantalla
	write(STDOUT_FILENO, "\n abriendo la fifo \n", sizeof("\n abriendo la fifo\n"));
	}//ahora va a abrir la fifo nombre, bandera y permisos
	fifo_d = open(FIFO_PATH, O_WRONLY, 0);// aca va un cero en lugar de los permisos xq la fifo ya fue creada arriba
	if(fifo_d == -1){//si no puede abrirla imprime lo sigte
	write(STDOUT_FILENO, "\n error al abrir fifo \n", sizeof("\n error al abrir fifo \n"));
	return -1;
	}else{//si logra abrirla imprime lo sigte
	write(STDOUT_FILENO, "\nfifo abierta correctamente \n", sizeof("\nfifo abierta correctamente \n"));
	}
	//se escribe en el fifo el mnsaje arriba definido con su tamaño
	err = write(fifo_d, MENSAJE, sizeof(MENSAJE));
	if(err == -1){
	write(STDOUT_FILENO, "\n error al escribir fifo \n", sizeof("\n error al escribir fifo \n"));
	return -1;
	}else{//si logra escribir, muestra lo escrito en pantalla
	write(STDOUT_FILENO, "\n escrito en fifo: \n", sizeof("\n escrito en fifo: \n"));
	write(STDOUT_FILENO, MENSAJE, sizeof(MENSAJE));
	}
	
	close(fifo_d);//cierra la fifo
	write(STDOUT_FILENO, "\ntermina proceso A\n", sizeof("\ntermina proceso A\n"));
	exit(0);//termina
}













