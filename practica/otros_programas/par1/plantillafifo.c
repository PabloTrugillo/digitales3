#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/wait.h> 

#define E "0123456789" //aca guarda el mensa q se va a enviar x la fifo en E
#define CC "/tmp/BB"//define la ruta de la fifo y el nombre CC
 int e,b,f,g;
 char d[10];
 int main(){
   mkfifo(CC, 0777);//aca se crea la fifo con nombre y permisos en octal
   b=fork();//aca hago un fork para tener un proc padr e hij y a la vez asigno a b el valor q devuelve fork
     if(b>0){//aca estoy posicionado en el proceso padre

	f = open(CC, O_WRONLY, 0);//el proc padre abre la fifo con permisos de solo escrit
	write(f, E,sizeof(E));//El proceso padre escribe "0123456789" en la FIFO
	wait(NULL);//El proc padre espera a que el proceso hijo termine.
	close(f);//El proceso padre cierra la FIFO
	unlink(CC);//El proceso padre elimina la FIFO
	exit(0);//El proceso padre termina el 
	} //al cerrar la llave salgo del proc padre

	e = open(CC, O_RDONLY, 0);//El proceso hijo abre la FIFO con permisos de solo lectura
	g = read(e, d, sizeof(d));//El proceso hijo lee de la FIFO
	write(STDOUT_FILENO, d, g);//El proceso hijo muestra lo leído
	close(e);//El proceso hijo cierra la FIFO
	exit(0);//El proceso hijo termina el
}


