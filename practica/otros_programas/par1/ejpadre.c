/*
 * PARCIAL 1: programa para ser ejecutado en el proceso padre.
 * 
 * Comando para compilar:
 * 
 * 		gcc -m32 -c padre.c && gcc -m32 hijo.o padre.o -o padre

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int hijo (int fd[2], char * myfifo);

int main(void) 
{
	int fd[2];//declara un arreglo de dos enteros para los descriptores de la pipe
	int pid;
	char tx_buffer[20] = {"HOLA HIJO MIO\0"};
    char * myfifo = "/tmp/myfifo";
	
    if (pipe(fd) == -1)// aca crea la pipe pasandole el descriptor fd, y pregunta si fue error
    {
        perror("pipe");
        return -1;
    }
    
    unlink(myfifo);// elimina fifo por si hay una creada con el mismo nombre
    
    if (mkfifo(myfifo, 0666) == -1)//aca crea la fifo con los campos de nombre y permisos(en octal), y pregunta si da error
    {
        perror("fifo");
        return -1;
    }
    
    if ((pid = fork()) == -1)//crea el proceso hijo con el fork, guarda su pid en la variable pid y pregunta si da error
    {
        perror("fork");
        return -1;
    }
    
// codigo que ejecuta el hijo

	if (pid==0) 
	{ 
        hijo(fd, myfifo);

        return 0;
    }

// codigo que ejecuta el padre   

	else
	{
		printf("Proceso Padre en ejecucion... \n"); 
		
		sleep(1);
				
		// **** ESCRIBIR CODIGO A PARTIR DE ESTA LINEA *****************
		//aca para la tuberia
		close(fd[0]);		 
		write(fd[1], tx_buffer, sizeof(tx_buffer));
		close(fd[1]);
		
		//aca para las señales
    		//kill(pid,SIGUSR1);
		//kill(pid,SIGUSR2);


		//ahora para la fifo hago lo siguiente
		//fd[1] = open(myfifo, O_WRONLY);
		//if(fd[1] == -1){
		//	write(STDOUT_FILENO, "\n error al abrir fifo \n", sizeof("\n error al abrir fifo \n"));
		//	return -1;
		//	}else{
		//	write(STDOUT_FILENO, "\nfifo abierta correctamente \n", sizeof("\nfifo abierta correctamente \n"));
		//	}
		//write(fd[1], tx_buffer, sizeof(tx_buffer));
		//close(fd[1]);
		// *************************************************************
		
		wait(NULL);
		
		return 0;
	}
}
