




#include "sapi.h"
#include "stdio.h"
#include "stdlib.h"
#include "../inc/freq_segment.h"
#include "../inc/segmentation.h"
#include "../inc/oversampling.h"
#include "arm_math.h"
#include "arm_const_structs.h"


/***************************************************/
/*
 *           VARIABLES GLOBALES
 */
/***************************************************/

extern header_struct_t header;

int main ( void ) {

	uint16_t	frenquency_bands[] = {F0, F1, F2, F3, F4};

	int16_t 	samples[N_DOWNSAMPLING];
	//fs=N_DOWNSAMPLING /CYCLES/K_CONSTANT/(MIN_BPM /BPM_TO_HZ);
	uint16_t	fs = 2792;

	strcpy(header.pre,"*header*");
	header.id=0;
	header.N=N_DOWNSAMPLING;
	header.fs = fs;
	strcpy(header.pos,"end*");

    /************ Configuraciones EDU-CIAA *************/
    boardConfig		 (							);
    uartConfig		 ( UART_USB, 460800			);
    adcConfig		 ( ADC_ENABLE				);
    cyclesCounterInit ( EDU_CIAA_NXP_CLOCK_SPEED );
    /***************************************************/

    while(1)
    {
    	cyclesCounterReset();

    	//TODO Calibrar el FAA de 10nF con una R de 3979 ohm para una fc de 4KHz, fc = 1/(2*pi*R*C)

    	/***----*** MOD 0 ***----***/
    	/*    OVERSAMPLING    */
    	oversampling( samples, N_DOWNSAMPLING, N_OVERSAMPLE, fs);

    	/***----*** MOD BPM ***----***/
 	    /*      FREQUENCY SEGMENTATION    */
    	/*      SMOOOTHING    */
    	freq_segmentation (samples,frenquency_bands, fs);
	}
}

