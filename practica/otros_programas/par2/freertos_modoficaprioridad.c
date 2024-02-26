//Complete la aplicación FreeRTOS para que cumpla las siguientes consignas:
//- Posea dos tareas: vTask1 y vTask2.
//- vTask1 periódicamente modifica la prioridad de vTask2 para que esta pueda ejecutarse.
//- vTask2 cambia el estado del LED verde y modifica su prioridad para que vTask1 retome la ejecución.
/********************************************************************************/
TaskHandle_t xTaskHandle;
int main( void )
{
	[xTaskCreate]( [vTask1], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+2], NULL );
	[xTaskCreate]( [vTask2], NULL, configMINIMAL_STACK_SIZE, NULL, [tskIDLE_PRIORITY+1], &xTaskHandle);
	[vTaskStartScheduler()];
	for( ;; );
	return 0;
}
/********************************************************************************/
void vTask1 ( void *pvParameters )
{
uint32_t i;
	for ( ;; )
	{
		for ( i=0; i<DELAY_LOOP_COUNT; i++) { vPrintString( "Espera activa\r\n" ); }
	[vTaskPrioritySet]( [xTaskHandle], [tskIDLE_PRIORITY+3] );
	}
}
/********************************************************************************/
void vTask2 ( void *pvParameters )
{
		for ( ;; )
		{
		Board_LED_Toggle(5); //LED Verde;
		[vTaskPrioritySet]( NULL, [tskIDLE_PRIORITY+1] );
		}
}
/********************************************************************************/
