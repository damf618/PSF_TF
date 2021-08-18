#include "sapi.h"
#include "stdio.h"
#include "stdlib.h"

#include "arm_math.h"
#include "../inc/smoothing.h"

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
   //q15_t max;
   char	pos[4];
} header_struct_t;



/***************************************************/
/*
 *           VARIABLES GLOBALES
 */
/***************************************************/
//header_struct_t header={"*header*",0,SAMPLES_LENGHT,AUDIO_FS,0,0,"end*"};

//extern q15_t Envelope[FREQ_RANGE][(uint16_t)ENVELOPE_LENGTH];

int main ( void ) {


    /************ Configuraciones EDU-CIAA *************/
    boardConfig		 (							);
    uartConfig		 ( UART_USB, 460800			);
    adcConfig		 ( ADC_ENABLE				);
    cyclesCounterInit ( EDU_CIAA_NXP_CLOCK_SPEED );
    /***************************************************/

    while(1)
    {
    	//q15_t FFT_Data[FREQ_RANGE][MAX_FFT_INDEX*2];
    	q15_t FFT_Data[MAX_FFT_INDEX*2];
    	arm_fill_q15(6,FFT_Data,MAX_FFT_INDEX*2);

    	smooth_set(FFT_Data);

	}
}

