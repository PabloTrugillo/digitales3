//Complete la aplicación FreeRTOS para que cumpla las siguientes consignas:
//- Vincular vTask1 y vTask2 a través de un mecanismo de sincronización.
//- vTask2 debe cambiar el estado del led verde cuando vTask1 finaliza un ciclo de espera activa.
//- vTask2 espera durante un segundo la señal de sincronización.
/********************************************************************************/
SemaphoreHandle_t xBinarySemaphore;
int main( void )
{
	[xBinarySemaphore] = [xSemaphoreCreateBinary ()];
/*Creación de tareas, inicio planificador*/
for( ;; );
return 0;
}
/********************************************************************************/
void vTask1 ( void *pvParameters )
{
	for ( ;; )
	{
		for ( i=0; i<DELAY_LOOP_COUNT; i++) { vPrintString( "Espera activa\r\n" ); }
	[xSemaphoreGive] ([xBinarySemaphore]);
	}
}

/********************************************************************************/
void vTask2 ( void *pvParameters )
{
	const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
	for ( ;; )
	{
		if( [xSemaphoreTake]( [xBinarySemaphore, xDelay] ) == pdTRUE)
		{
			Board_LED_Toggle(5); //LED Verde;
		}	
		else
		{
		Board_LED_Toggle(4); //LED Rojo;
		}
	}
}
/********************************************************************************/
