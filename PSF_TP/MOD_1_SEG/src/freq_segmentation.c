#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "../inc/freq_segmentation.h"
#include "../inc/fir.h"




/***************************************************/
void seg_set(q15_t* FFT)
{
	//TODO Implementar un tipo de Ventana para suavizar los cortes de la senial
	uint16_t index_min;
	uint16_t index_max;
	uint16_t index = 0;
	uint16_t Freq_Ranges[FREQ_RANGE] = {F0,F1,F2,NYQUIST_SAMPLING};
	uint16_t FFT_LEN = MAX_FFT_INDEX * 2;

	//SEGMENTACION POR FRECUENCIA
	for (uint16_t i=0;i<FREQ_RANGE-1;i++)
	{
		//  Si 1024 son fs/2 entonces X es F0/F1/...
		index_min = (2*FFT_LEN*Freq_Ranges[i])/SUB_SAMPLING_FREQ;
		index_max = (2*FFT_LEN*Freq_Ranges[i+NEXT])/SUB_SAMPLING_FREQ;
		index = 0;

		//indice minimo y maximo de las frecuencias minimo y maximo.
		for(uint16_t j= index_min;j<index_max;j++)
		{
			//Almacenamiento de Rango de FFT Correspondiente al rango
			mod1_signals[i][index] = FFT[j];
			index++;
		}

		//RELLENAMOS CON CEROS
		for(uint16_t j= index;j<FFT_LEN;j++)
		{
			mod1_signals[i][j] = 0;
		}
	}
}


/***************************************************/
void Data_SubSampling(q15_t * samples)
{
	const uint16_t rate = AUDIO_FS/SUB_SAMPLING_FREQ;
	const uint16_t sub_sampling = (uint16_t) SUB_SAMPLING_FREQ*(CYCLES/MIN_BPM_FREQ);

	arm_rfft_instance_q15 FFT_ins;
	arm_status status;

	q15_t original_signal[MAX_FFT_INDEX];
	q15_t FFT_original_signal[MAX_FFT_INDEX * 2];			// Resultado en Numeros Complejos
	q15_t filtered_signal [SAMPLES_LENGHT + h_LENGTH -1];	// Resultado de la convolucion es de largo N+M-1


	//FILTRADO PASABAJO DE samples
	arm_conv_fast_q15  ( samples,SAMPLES_LENGHT,h,h_LENGTH,filtered_signal);

	//SUBSAMPLING DE samples
	for(uint16_t i=0;i<sub_sampling*rate;i=i+rate)
	{
		original_signal[i/rate] = filtered_signal[i];
	}

	//ZERO PADDING subsampling
	for(uint16_t i=sub_sampling;i<MAX_FFT_INDEX;i++)
	{
		original_signal[i] = 0;
	}

	//FFT subsampling
	arm_rfft_init_q15(&FFT_ins, MAX_FFT_INDEX, 0, 1);
	if(ARM_MATH_ARGUMENT_ERROR == status)
	{
		while(1);
	}
	arm_rfft_q15(&FFT_ins, original_signal, FFT_original_signal);

	//SEGMENTACION DE FFT
	seg_set(FFT_original_signal);
}


/***************************************************/
void get_data(void)
{
	q15_t data_adc [SAMPLES_LENGHT];
	uint32_t index =0;
	int16_t analog_read;
	char data_acquired = FALSE;

	while(!data_acquired)
	{
		cyclesCounterReset();

		analog_read = (int16_t )(adcRead(CH1)-512);		   // medicion adc ch1 centrada a 0, por el divisor de tension.
		analog_read = analog_read <<6;					   // shift de 6 bits para un formato de q15
		data_adc[index]	= analog_read;

		if ( ++index == SAMPLES_LENGHT)
		{
			data_acquired = TRUE;
			gpioToggle ( LEDR );						   // este led blinkea a fs/N

			Data_SubSampling(data_adc);

			adcRead(CH1); 								   // Lectura misteriosa
		}
		gpioToggle ( LED1 );											 // este led blinkea a fs/2
		while(cyclesCounterRead()< EDU_CIAA_NXP_CLOCK_SPEED/AUDIO_FS); // el clk de la CIAA es 204000000
	}
}


