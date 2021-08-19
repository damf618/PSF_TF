#include "sapi.h"
#include "stdio.h"
#include "stdlib.h"
#include "../inc/freq_segment.h"
#include "../inc/segmentation.h"
#include "../inc/oversampling.h"

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
   char	pos[4];
} header_struct_t;

/***************************************************/
/*
 *           VARIABLES GLOBALES
 */
/***************************************************/

int main ( void ) {

	uint16_t	frenquency_bands[] = {F0,F1, F2, F3, F4};

	int16_t 	samples[N_DOWNSAMPLING];
	uint16_t	fs = N_DOWNSAMPLING * (CYCLES/K_CONSTANT) / (MIN_BPM / BPM_TO_HZ);

	int16_t 	segmented_signals[N_FREQUENCIES-1][N_DOWNSAMPLING];

	header_struct_t header={"*header*",0,N_DOWNSAMPLING,fs,"end*"};


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

    	/***----*** MOD 1 ***----***/
 	    /*      FREQUENCY SEGMENTATION    */
    	freq_segmentation (samples,segmented_signals,frenquency_bands, fs);

    	/***----*** MOD 2 ***----***/
 	    /*      SMOOOTHING    */
    	//SMOOTHING
	}
}

