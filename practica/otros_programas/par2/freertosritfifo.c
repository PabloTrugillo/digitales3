//Complete la aplicación FreeRTOS para que cumpla las siguientes consignas:
//- En cada interrupción del RIT se debe enviar a través de una cola de mensajes FIFO el valor entero 123.
//- La tarea vTask1 de alta prioridad debe recibir los datos enviados por la interrupción.
//- En el caso de que la cola esté vacía, vTask1 se debe bloquear por tiempo indefinido.
/********************************************************************************/
QueueHandle_t xQueue;
int main( void )
{
	UBaseType_t uxQueueLength = 10;
	UBaseType_t uxItemSize = sizeof( int32_t );
	xQueue = [xQueueCreate]( [uxQueueLength, uxItemSize] );
	/*Creación de tareas, habilitación de interrupciones, inicio del planificador*/
	for( ;; );
	return 0;
}
/********************************************************************************/
void vTask1 ( void *pvParameters )
{
	int32_t lReceivedValue;;
	const TickType_t xDelay = [pdMS_TO_TICKS]( 1000 );
	for ( ;; )
	{
	[xQueueReceive]( xQueue, [&lReceivedValue, portMAX_DELAY] );
	}
}
/********************************************************************************/
void vTask2 ( void *pvParameters )
{
	for ( ;; ) { vPrintString( "Espera activa\r\n" ); }
{
/********************************************************************************/
void RIT_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	int32_t lValueToSend = 123;
	[xQueueSendToBackFromISR]( xQueue, [&lValueToSend], &xHigherPriorityTaskWoken );
	if( [xHigherPriorityTaskWoken] == pdTRUE ){
	[portYIELD_FROM_ISR]( [xHigherPriorityTaskWoken] )
	}
	Chip_RIT_ClearInt(LPC_RITIMER);
}
/********************************************************************************/