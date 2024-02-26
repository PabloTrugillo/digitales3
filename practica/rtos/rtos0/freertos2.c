/*  Copyright 2020 - FreeRTOS Kernel V10.0.1

 * este codigo NO esta en el libro de Sistemas Empotrados en tiempo real
 * pero el objetivo es intriducir a FreeRTOS usando solo una  tarea que 
 * imprime por el puerto serie, e invocando al planificador
 * algo de manejo de TICKs
    - vTaskDelay()
    - pdMS_TO_TICKS

*/

#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

/* Demo includes. */
#include "supporting_functions.h"

/* The task function. */

void vTaskFunction( void *pvParameters )
{
   TickType_t xLastWakeTime;
   const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );

   printf ( "tics de delay: %d\r\n",xDelay500ms );
   //printf ( "cuantos tics por ms: %d\r\n",portTICK_RATE_MS );//aca se imprime el valor en el q esta definido portTICK_RATE_MS

   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ) {
      vPrintString( "Tarea 1\r\n" );
      xLastWakeTime = xTaskGetTickCount();//obtiene el valor del tiks actual, q en este caso es uno cada 5o ticks, es cada vez q se despierta la tarea ya q el vTaskDelay la duerme por 50 ticks, en la sigte linea imprime ese vaolr.
      printf ( "time %d\r\n",xLastWakeTime );

//    vTaskDelay( 500 / portTICK_RATE_MS );// el 500 representa el valor de tiempo en ms que nosotros le pedimos y el portTICK_RATE_MS la cantidad de ticks por segundo. en este caso es 500/1=500ms
      vTaskDelay( xDelay500ms );//entre () de esta fcion va el tiempo que queremos mandar a la tarea al estado de bloqueado.                    
   }
}

/*-----------------------------------------------------------*/

int main( void )
{
   /* Crea tarea con stack minimo y prioridad idle + 1 ... */
   xTaskCreate( vTaskFunction, "Task 1", configMINIMAL_STACK_SIZE, NULL , tskIDLE_PRIORITY+1, NULL );


	vTaskStartScheduler(); /* y por último se arranca el planificador . */
    //Nunca llegara a ese lazo  .... espero

   for( ;; );
   return 0;
}
/*-----------------------------------------------------------*/


