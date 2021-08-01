#include "sapi.h"
#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "math.h"
#include "arm_const_structs.h"

//#define SAMPLES_LENGHT 2048
#define SAMPLES_LENGHT 1024

#define AUDIO_FS 44100
//#define AUDIO_FS 8000

/* ------------------------------------------------------------------
* Global variables for FIR Example
* ------------------------------------------------------------------- */
#define BLOCK_SIZE     32
#define FILTER_2K_TAPS 50
const q15_t firCoeffsq15[FILTER_2K_TAPS] = { 0x0102, 0x016D,  0x01B0,  0x01C2,  0x0199,  0x0137,  0x00A0,  0xFFE4,  0xFF14,
		0xFE49,  0xFD9D,  0xFD29,  0xFD06,  0xFD44,  0xFDEF,  0xFF07,  0x0084,  0x0253,  0x045C,  0x067C,  0x088F,  0x0A6F,
		0x0BFB,  0x0D15,  0x0DA7,  0x0DA7,  0x0D15,  0x0BFB,  0x0A6F,  0x088F,  0x067C,  0x045C,  0x0253,  0x0084,  0xFF07,
		0xFDEF,  0xFD44,  0xFD06,  0xFD29,  0xFD9D,  0xFE49,  0xFF14,  0xFFE4,  0x00A0,  0x0137,  0x0199,  0x01C2,  0x01B0,
		0x016D,  0x0102
};

static q15_t firStateq15[BLOCK_SIZE + FILTER_2K_TAPS];
arm_fir_instance_q15  varInstFIRq15;
q15_t firresult [SAMPLES_LENGHT];

typedef struct header_struct {
   char		pre[8];
   uint32_t id;
   uint16_t N;
   uint16_t fs;
   uint32_t index;
   q15_t max;
   char	pos[4];
} __attribute__ ((packed));

struct header_struct header={"*header*",0,SAMPLES_LENGHT,AUDIO_FS,0,0,"end*"};


/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = SAMPLES_LENGHT/2+1;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
arm_rfft_instance_q15 varInstRfftq15;
q15_t fftresult [SAMPLES_LENGHT*2];
q15_t fftMag[ SAMPLES_LENGHT /2+1 ]; // magnitud de la FFT


q15_t adc [ SAMPLES_LENGHT];
q15_t sample_fft [ SAMPLES_LENGHT];

int main ( void ) {


	uint32_t blockSize = BLOCK_SIZE;
	uint32_t numBlocks = SAMPLES_LENGHT/BLOCK_SIZE;

	arm_status status;

    uint16_t sample = 0;
    int16_t data = 0;
    boardConfig		 (							);
    uartConfig		 ( UART_USB, 460800			);
    adcConfig		 ( ADC_ENABLE				);
    cyclesCounterInit ( EDU_CIAA_NXP_CLOCK_SPEED );

    while(1)
    {
	    cyclesCounterReset();

	    uartWriteByteArray ( UART_USB ,(uint8_t* )&adc[sample]                  ,sizeof(adc[0]) );
	    uartWriteByteArray ( UART_USB ,(uint8_t* )&firresult[sample]                  ,sizeof(firresult[0]) );
	    uartWriteByteArray ( UART_USB ,(uint8_t* )&fftresult[sample*2]	,sizeof(fftresult[0])); // envia la fft del sample ANTERIO
	    uartWriteByteArray ( UART_USB ,(uint8_t* )&fftresult[sample*2+1] ,sizeof(fftresult[0])); // envia la fft del sample ANTERIO

	    data = (int16_t )(adcRead(CH1)-512);		   // adc -> 10 bits,si deseo un q15...debo shiftearlo hacia el msb 6 posiciones
	    data = data <<6;
	    adc[sample] = data;
	    //sample_fft[sample] = data;

	    if ( ++sample==header.N )
        {
		   gpioToggle ( LEDR ); // este led blinkea a fs/N
		   sample = 0;

		   status = arm_fir_init_q15(&varInstFIRq15, FILTER_2K_TAPS, (q15_t *)&firCoeffsq15[0], &firStateq15[0], blockSize);
		   if(ARM_MATH_ARGUMENT_ERROR == status)
		   {
		       while(1);
		   }

		   for(uint16_t i=0; i < numBlocks; i++)
		   {
		       arm_fir_q15(&varInstFIRq15, &adc[i * blockSize], &firresult[i * blockSize], blockSize);
		   }

		   memcpy(sample_fft,firresult,sizeof(firresult));



		   status = arm_rfft_init_q15(&varInstRfftq15,SAMPLES_LENGHT,ifftFlag,doBitReverse );
		   // inicializa una estructira que usa la funcion fft para procesar los datos. Notar el /2 para el largo
		   // Process the data through the CFFT/CIFFT module
		   if(ARM_MATH_ARGUMENT_ERROR == status)
		   {
		       while(1);
		   }

		   arm_rfft_q15(&varInstRfftq15, sample_fft, fftresult);
		   /* Process the data through the Complex Magnitude Module for
		   calculating the magnitude at each bin */

		   arm_cmplx_mag_q15(fftresult, fftMag, fftSize);
		   /* Calculates maxValue and returns corresponding BIN value */
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

