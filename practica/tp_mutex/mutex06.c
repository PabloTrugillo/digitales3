/* Ejercicio 7 del TP mutex  productor-consumidor con mutex */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_BUFFER 5		//tamanio del buffer
 

pthread_mutex_t mutex;      // mutex de acceso al buffer compartido
int cant;                 	// numero de elementos en el buffer 
int buffer[MAX_BUFFER];     // buffer comun 
int DATOS_A_PRODUCIR=20;

///////////////////////////////////////////////////////////////

void Productor() {     //PRODUCTOR 

int dato, i, pos;
	pos=0;
	dato=1000;
	i=0;

	while( i < DATOS_A_PRODUCIR ) {//aca el hilo productor va a producir desde i=0 hasta la cantidad de datos a producir que es 20
	
		sleep(1);	//este retardo es para ver como funciona con mutex
		
		pthread_mutex_lock(&mutex);//resto el mutex para acceder al recurso compartido   
		//a coninuacion lo va a ir haciendo hasta el tamaño maximo del buffer, o sea 5
		while (cant < MAX_BUFFER){ 		//si buffer lleno
		//a coninuacion va a ir escribiendo en una posicion del buffer y pasando a la sigte
			buffer[pos] = dato;
			dato++;
			pos = (pos + 1) ;
			if (pos>= MAX_BUFFER) {
				pos=0;	}
			cant++;
			i++;
			printf("Productor posicion: %d, dato: %d, cantidad: %d, i: %d\n",pos,dato,cant,i);	 
		}
		
		pthread_mutex_unlock(&mutex);//al salir del recurso compartido sumo el mutex	     
	}
		printf("Termino produccion: %d\n",i);
		pthread_exit(0);
}

///////////////////////////////////////////////////////////////

void Consumidor(){    // CONSUMIDOR

int dato, i, pos;
	pos=0;
	i=0;
	
	while( i < DATOS_A_PRODUCIR ) {//aca el hilo consumidor va a leer datos desde i=0 hasta la cantidad de datos a producir que es 20
	
		pthread_mutex_lock(&mutex);//resto el mutex para acceder a la variable compartida 	
		//a continuacion leo de a 5 datos del buffer
		while (cant > 0){ 		// si buffer vacio
			dato = buffer[pos];
			pos = (pos + 1) ;
			if (pos > (MAX_BUFFER-1)) {
				pos=0;	}
			cant--;
			i++;
			printf("Consumidor posicion: %d, dato: %d, cantidad: %d, i: %d\n",pos,dato,cant,i);	 
		}
		
		pthread_mutex_unlock(&mutex);//al salir de la variable compartida sumo el mutex	     
	}
	printf("Termino Consumo: %d\n",i);
	
	pthread_exit(0);
}
 ///////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

pthread_t th1, th2;

	pthread_mutex_init(&mutex, NULL);//inicializa un mutex dinamico con los atributos por defecto
	
	if (argc == 2){
		printf ("argv[0]= %s , argv[1]= %s ,  argc=%d\n", argv[0], argv[1], argc);//esto es para mostrar los argumentos en caso de tenerlos
		DATOS_A_PRODUCIR=atoi(argv[1]);
	}
	//a continuacion creo los dos hilos
	pthread_create(&th1, NULL, (void *)&Productor, NULL);
	pthread_create(&th2, NULL, (void *)&Consumidor, NULL);
	//a continuacion espero a que los dos hilos terminen
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	
	printf("Productor-Consumidor con mutex: termina\n");
	
	pthread_exit(0);


}


