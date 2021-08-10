#include "sapi.h"
#include "stdio.h"
#include "stdlib.h"
#include "../inc/freq_segmentation.h"

/***************************************************/
/*
 *           CONFIGURACION DE SAMPLEO
 */
/***************************************************/




/***************************************************/
/*
 *           ESTRUCTURA DE PAQUETE UART
 */
/***************************************************/
typedef struct header_struct
{
   char		pre[8];
   uint32_t id;
   uint16_t N;
   uint16_t fs;
   uint32_t index;
   q15_t max;
   char	pos[4];
} header_struct_t;



/***************************************************/
/*
 *           VARIABLES GLOBALES
 */
/***************************************************/
header_struct_t header={"*header*",0,SAMPLES_LENGHT,AUDIO_FS,0,0,"end*"};

int main ( void ) {


    /************ Configuraciones EDU-CIAA *************/
    boardConfig		 (							);
    uartConfig		 ( UART_USB, 460800			);
    adcConfig		 ( ADC_ENABLE				);
    cyclesCounterInit ( EDU_CIAA_NXP_CLOCK_SPEED );
    /***************************************************/

    while(1)
    {
    	get_data();
	    //uartWriteByteArray ( UART_USB ,(uint8_t* )&adc      [sample],     sizeof(adc[0]      )); // UART sample actual
	}
}

