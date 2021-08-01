#include "../inc/freq_segmentation.h"
#include "sapi.h"
#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "math.h"
#include "arm_const_structs.h"

/***************************************************/
/*
 *           CONFIGURACION DE SAMPLEO
 */
/***************************************************/
#define SAMPLES_LENGHT 1024
#define AUDIO_FS 44100


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

/* Global variables for FFT */
uint32_t fftSize = SAMPLES_LENGHT/2+1;                  // FFT va de -n/2 a n/2, al ser RFFT -> [0,n/2+1]
uint32_t ifftFlag = 0;                                  // Configuracion CMSIS-DSP *IFFT
uint32_t doBitReverse = 1;                              // Configuracion CMSIS-DSP *
arm_rfft_instance_q15 varInstRfftq15;                   // Instancia CMSIS-DSP RFFT
q15_t fftresult [SAMPLES_LENGHT*2];                     // Resultado FFT puede ser complejo n puntos Re e Im
q15_t fftMag[ SAMPLES_LENGHT /2+1 ];                    // Magnitud de la FFT de complejos a un numero

/* Global variables for Samples */
q15_t adc [ SAMPLES_LENGHT];							// Mediciones ADC
q15_t sample_fft [ SAMPLES_LENGHT];						// Mediciones de respaldo de ADC

int main ( void ) {

	arm_status status;									// Validacion de inicializaciones de CMSIS-DSP
    uint16_t sample = 0;								// indice de mediciones
    int16_t data = 0;									// variable aux para debug y mediciones

    /************ Configuraciones EDU-CIAA *************/
    boardConfig		 (							);
    uartConfig		 ( UART_USB, 460800			);
    adcConfig		 ( ADC_ENABLE				);
    cyclesCounterInit ( EDU_CIAA_NXP_CLOCK_SPEED );
    /***************************************************/

    while(1)
    {
	    cyclesCounterReset();

	    uartWriteByteArray ( UART_USB ,(uint8_t* )&adc      [sample],     sizeof(adc[0]      )); // UART sample actual
	    uartWriteByteArray ( UART_USB ,(uint8_t* )&fftresult[sample*2],   sizeof(fftresult[0])); // FFT parte Re
	    uartWriteByteArray ( UART_USB ,(uint8_t* )&fftresult[sample*2+1], sizeof(fftresult[0])); // FFT parte Im

	    data = (int16_t )(adcRead(CH1)-512);		   // medicion adc ch1 centrada a 0, por el divisor de tension.
	    data = data <<6;							   // shift de 6 bits para un formato de q15
	    adc[sample] = data;                            // carga de matriz de samples
	    sample_fft[sample] = data;                     // carga de matriz de respaldo de samples

	    if ( ++sample==header.N )
        {
		   gpioToggle ( LEDR );						   // este led blinkea a fs/N
		   sample = 0;

		   //Inicializacion de estructura
		   status = arm_rfft_init_q15(&varInstRfftq15,SAMPLES_LENGHT,ifftFlag,doBitReverse );
		   if(ARM_MATH_ARGUMENT_ERROR == status)
		   {
		       while(1);
		   }

		   //CMSIS-DSP RFFT a los Samples
		   arm_rfft_q15(&varInstRfftq15, sample_fft, fftresult);

		   //Calculo de Amplitud de numeros complejos
		   arm_cmplx_mag_q15(fftresult, fftMag, fftSize);

		   //Busqueda de valor max y su indice
		   arm_max_q15(fftMag, fftSize, &header.max, &header.index);

		   //trigger(2);
		   header.id++;
		   uartWriteByteArray ( UART_USB ,(uint8_t*)&header ,sizeof(struct header_struct ));
		   adcRead(CH1); //why?? hay algun efecto minimo en el 1er sample.. puede ser por el blinkeo de los leds o algo que me corre 10 puntos el primer sample. Con esto se resuelve.. habria que investigar el problema en detalle
	    }
	gpioToggle ( LED1 );											 // este led blinkea a fs/2
	while(cyclesCounterRead()< EDU_CIAA_NXP_CLOCK_SPEED/header.fs); // el clk de la CIAA es 204000000
    }
}

