
#ifndef SMOOTHING
#define SMOOTHING

#include "arm_math.h"

/***************************************************/
/*
 *          DEFINE Segmentacion de Frecuencia
 */
/***************************************************/
#define F0					0						//Rango 1 Instrumentos Baja          Freq
#define F1					200						//Rango 1 Instrumentos Baja          Freq
#define F2					400						//Rango 2 Instrumentos Baja / Media  Freq
//#define F3					800						//Rango 3 Instrumentos Media         Freq
//#define F4 					1600					//Rango 4 Instrumentos Alta / Media  Freq
//#define F5					3200					//Rango 5 Instrumentos Alta          Freq
#define FREQ_RANGE   		4						// Numero de Segmentos definidos


#define SAMPLES_LENGHT 		5000					//Samples de primera etapa Adquisicion de senial
#define	SUB_SAMPLING_FREQ	1360					//Freq de SubSampling 5 veces menor que la de sampleo.
#define	NYQUIST_SAMPLING	SUB_SAMPLING_FREQ/2
#define AUDIO_FS 			6800					//IDEAL > 20KHz, se limita a 6.8KHz para 50000 Samples

#define CYCLES 				2.2						//Ciclos minimos de senial (BPM) a samplear
#define MIN_BPM_FREQ		3						// 180 BPM / 60 segundos

#define MAX_FFT_INDEX		1024

#define NEXT       			1

//const uint16_t sub_sampling = (uint16_t) SUB_SAMPLING_FREQ*(CYCLES/MIN_BPM_FREQ);
//const uint16_t zero_padding = (uint16_t) MAX_FFT_INDEX-sub_sampling;
//const uint16_t range_length	= (uint16_t) MAX_FFT_INDEX/FREQ_RANG;

/***************************************************/
/*
 *          DEFINE Suavizacion
 */
/***************************************************/

#define WINDOW_SECONDS		0.4
#define WINDOW_LENGTH		544										//SUB_SAMPLING_FREQ*WINDOW_SECONDS
#define ENVELOPE_LENGTH		1296									//MAX_FFT_INDEX*2+WINDOW_LENGTH/2-1
#define TIME_DOMAIN			1
#define IFFT_LENGTH			4096

/***************************************************/
/*
 *          Estructuras y Tipos de Variables
 */
/***************************************************/


/***************************************************/
/*
 *                 Variables
 */
/***************************************************/

//q15_t Envelope[FREQ_RANGE][ENVELOPE_LENGTH];

/***************************************************/
/*
 *          Prototipos de Funciones
 */
/***************************************************/
//void smooth_set(q15_t FFT_Data[FREQ_RANGE][MAX_FFT_INDEX*2]);
void smooth_set(q15_t FFT_Data[MAX_FFT_INDEX*2]);

#endif
