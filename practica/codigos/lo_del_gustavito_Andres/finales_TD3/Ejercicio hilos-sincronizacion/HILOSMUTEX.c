#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h>
#define MENSAJE1 "0123456789"
#define MENSAJE2 "ABCDEFGHIJ"

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER; // inicializacion estatica del mutex
char buff[256],t=0;
pthread_t hilo[2];
int rc,arre[2],s;
sem_t sem;


void *hiloA(void *a){

/*s=pthread_mutex_lock(&mtx); //lock (resta 1)  
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }
*/

strncpy(buff,MENSAJE1,sizeof(MENSAJE1));

printf("mensaje copiado\n");

/*s=pthread_mutex_unlock(&mtx); //suma 1 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);    }
*/

//------ Se incrementa sem 
	s=sem_post(&sem);
	if (s != 0) {
		printf("ERROR sem_post()\n");
		exit(-1);     }

pthread_exit(NULL);
}
void *hiloB(void *b){

/*s=pthread_mutex_lock(&mtx); //lock (resta 1)  
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   }
*/

strncpy(buff,MENSAJE2,sizeof(MENSAJE2));
printf("mensaje copiado\n");

/*s=pthread_mutex_unlock(&mtx); //suma 1 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);    }
*/
//------ Se incrementa sem 
	s=sem_post(&sem);
	if (s != 0) {
		printf("ERROR sem_post()\n");
		exit(-1);     }

pthread_exit(NULL);
}

int main() {

//------ inicializa el sem sin nombre 
	s=sem_init(&sem, 0, 0);
	if (s != 0) {
		printf("ERROR sem_init()\n");
		exit(-1);     }

printf("Main creando el hilo nro %d\n", t);
     arre[t] = t;
     
     //rc = pthread_create(&hilo[t], NULL, hola , (void *)(&t)  );
     rc = pthread_create(&hilo[t], NULL, hiloA , (void *) (&arre[t]) );
     
     if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);   }     
//sleep(2);
//pthread_join(hilo[t],NULL);
/*s=pthread_mutex_lock(&mtx); //suma 1 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);  }  
*/
//------decremento de sem
	s = sem_wait(&sem);
	if (s != 0) {
		printf("ERROR sem_wait()\n");
		exit(-1);     }

printf(" El mensaje recibido es %s y fue enviado por el hilo[%d]A\n",buff,t);
t++;

/*s=pthread_mutex_unlock(&mtx); //suma 1 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);    }
*/
printf("Main creando el hilo nro %d\n", t);
     arre[t] = t;
     rc = pthread_create(&hilo[t], NULL, hiloB , (void *) (&arre[t]) );
     
     if (rc != 0){
         printf("ERROR; pthread_create() = %d\n", rc);
         exit(-1);        }

//pthread_join(hilo[t],NULL);
//sleep(2);
/*s=pthread_mutex_lock(&mtx); //suma 1 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);   } 
*/
//------decremento de sem
	s = sem_wait(&sem);
	if (s != 0) {
		printf("ERROR sem_wait()\n");
		exit(-1);     }

printf(" El mensaje recibido es %s y fue enviado por el hilo[%d]B\n",buff,t);

/*s=pthread_mutex_unlock(&mtx); //suma 1 
   	if (s!=0){
      printf("ERROR; pthread_mutex() = %d\n", s);
      exit(-1);  }
*/
//sleep(2);
exit(0);
}


