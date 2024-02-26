//Complete la aplicación FreeRTOS para que cumpla las siguientes consignas:
//1- Posea una tarea única con la prioridad más baja posible.
//2- La tarea debe cambiar el estado del LED rojo, enviar por puerto serie el texto recibido como parámetro en su
//creación y bloquearse durante 800ms.
/********************************************************************************/
const char *pvTaskParameters = "Tarea 1 en ejecución\r\n";
int main( void )
{
xTaskCreate( [vTask1], NULL, configMINIMAL_STACK_SIZE, (void*)pvTaskParameters, tskIDLE_PRIORITY+1,NULL );
vTaskStartScheduler();
for( ;; );
return 0;
}
/********************************************************************************/
void vTask1 ( void *pvParameters )
{
char *pcTaskName;
const TickType_t xDelay = [pdMS_TO_TICKS]( 800 );
pcTaskName = ( char * ) [pvParameters];
for( ;; )
{
vPrintString( pcTaskName );
Board_LED_Toggle(4); //LED rojo;
vTaskDelay( xDelay );
}
}
