/*  Copyright 2020 - FreeRTOS Kernel V10.0.1

    codigo basado en el libro Sistemas Empotrados en tiempo real 
    José Daniel Muñoz Frías
4.9.  Gestión de tiempo
4.9.2.  Ejemplo: arranque de una bomba (Pagina 124)

La función ArrancaBomba arranca una
bomba mediante un arrancador estrella/triángulo y después de un tiempo
monitoriza la presión de salida de la bomba para verificar que ésta está
funcionando correctamente. El tiempo de retardo desde que se conecta el
motor en triángulo hasta que se conecta en estrella es de 500 ms, mientras
que el tiempo que se espera antes de verificar si se está bombeando es igual
a 1 minuto, por si acaso la bomba estaba descargada. //cambiado por 6 segundos
    usa funcion vTaskDelay()

 */

/*==================[inclusions]=============================================*/

#include "board.h"
#include "chip.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"


/*==================[macros and definitions]=================================*/

#define PRIO_ARRANQUE 2
#define TAM_PILA 512

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void InitHardware(void)
{
    SystemCoreClockUpdate();
    Board_Init();
}

void ConectaTensionEstrella(void)
{
	Board_LED_Set(3, TRUE); //enciende "LED 1" ( amarillo )
}

void ConectaTensionTriangulo(void)
{
	Board_LED_Set(5, TRUE); //enciende "LED 3" ( verde )
}

void DesconectaTension(void)
{
	Board_LED_Set(3, FALSE); //apaga "LED 2" ( amarillo )
	Board_LED_Set(5, FALSE); //apaga "LED 3" ( verde )
}

void AlarmaFalloArranque(void)
{
	Board_LED_Set(4, TRUE); //enciende "LED 2" ( rojo )
}

uint8_t PresionOK(void)
{
	return (Board_TEC_GetStatus(BOARD_TEC_1));
}

void ArrancaBomba(void)
{
	ConectaTensionEstrella();
	vTaskDelay (500/portTICK_RATE_MS);
	ConectaTensionTriangulo();
	vTaskDelay (6000/portTICK_RATE_MS); //6 segundros
	if( PresionOK()==0){ /* No hay presión . Por tanto la
							bomba no está funcionando */
		DesconectaTension();
		AlarmaFalloArranque();
	}
}

void Arranque(void *pvParameters)
{
	while(1){
		ArrancaBomba();
		vTaskDelay (10000/portTICK_RATE_MS); //espera 10 segundo y hace todo de nuevo
		Board_LED_Set(4, FALSE); //apaga "LED 2" ( rojo )
	}
}


/*==================[external functions definition]==========================*/

int main(void)
{
	InitHardware(); /* Inicializa el Hardware del microcontrolador */

	/* Se crean las tareas */
	xTaskCreate(Arranque, (const char *)"Arranque", TAM_PILA, NULL, PRIO_ARRANQUE, NULL );

	vTaskStartScheduler(); /* y por último se arranca el planificador . */
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
