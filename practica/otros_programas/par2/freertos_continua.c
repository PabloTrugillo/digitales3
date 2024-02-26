//Complete la aplicación FreeRTOS para que cumpla las siguientes consignas:
//1- Posea una tarea vTask1 que se ejecute de manera continua, informando este estado por el puerto serie.
//2- vTask1 se debe crear en primer lugar.
//3- Posea una tarea vTask2 que periódicamente cambie el estado del LED verde y se bloquee durante 750ms.
int main( void )
{
	[xTaskCreate]( [vTask1], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+2], NULL );
	[xTaskCreate]( [vTask2], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+3], NULL );
	[vTaskStartScheduler();]
	for( ;; );
	return 0;
}
void vTask1 ( void *pvParameters )
{
	for( ;; ) {
	vPrintString( "Tarea continua en ejecución\r\n" );;
	}
}
void vTask2 ( void *pvParameters )
{
	const TickType_t xDelay = [pdMS_TO_TICKS]( 750 );
	for( ;; ) {
	Board_LED_Toggle(5); //LED verde;
	[vTaskDelay]( [xDelay] );
}
}
