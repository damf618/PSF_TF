
#ifndef OVER_SAMPLING
#define OVER_SAMPLING

#include "arm_math.h"
#include "sapi.h"


/*
#define SAMPLES_LENGHT 		5000					//Samples de primera etapa Adquisicion de senial
#define	SUB_SAMPLING_FREQ	1360					//Freq de SubSampling 5 veces menor que la de sampleo.
#define	NYQUIST_SAMPLING	SUB_SAMPLING_FREQ/2
#define AUDIO_FS 			6800					//IDEAL > 20KHz, se limita a 6.8KHz para 50000 Samples

//#define CYCLES 				2.2						//Ciclos minimos de senial (BPM) a samplear
#define MIN_BPM_FREQ		3						// 180 BPM / 60 segundos

#define MAX_FFT_INDEX		1024

#define NEXT       			1
*/
//const uint16_t sub_sampling = (uint16_t) SUB_SAMPLING_FREQ*(CYCLES/MIN_BPM_FREQ);
//const uint16_t zero_padding = (uint16_t) MAX_FFT_INDEX-sub_sampling;
//const uint16_t range_length	= (uint16_t) MAX_FFT_INDEX/FREQ_RANG;

/***************************************************/
/*
 *          Prototipos de Funciones
 */
/***************************************************/
void oversampling( q15_t* down, uint16_t n_down, uint8_t over_factor, uint16_t fs);

#endif	//OVER_SAMPLING
