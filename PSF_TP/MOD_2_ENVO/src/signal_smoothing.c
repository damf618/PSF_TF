#include "stdio.h"
#include "stdlib.h"
#include "../inc/smoothing.h"
#include "../inc/hanning.h"

/***************************************************/
void FFT_conv_q15(q15_t * FFT, q15_t* Result, uint8_t time)
{
	q15_t FFT_PADD    [IFFT_LENGTH];
	//q15_t filter_PADD [IFFT_LENGTH];

	arm_status status;
	arm_rfft_instance_q15 rfft_instance;


	//ZERO PADDING FFT desde MAX_FFT_INDEX*2  hasta ENVELOPE_LENGTH
	arm_fill_q15(0,FFT_PADD,IFFT_LENGTH);

	memcpy(FFT_PADD,FFT,MAX_FFT_INDEX*2*sizeof(q15_t));			//FFT LEN

	if(time)
	{
		q15_t CONV_result [IFFT_LENGTH];
		//TEOREMA DE CONVOLUCION
		arm_cmplx_mult_cmplx_q15(FFT_PADD,hanning,CONV_result,IFFT_LENGTH);

		//MOD3 - IFTT Result
		status = arm_rfft_init_q15(&rfft_instance, IFFT_LENGTH, 1, 1);
		if(status == ARM_MATH_ARGUMENT_ERROR )
		{
			while(1);
		}
		arm_rfft_q15(&rfft_instance, CONV_result,Result);
	}
	else
	{
		//TEOREMA DE CONVOLUCION
		arm_cmplx_mult_cmplx_q15(FFT_PADD,hanning,Result,ENVELOPE_LENGTH);
	}
}



/***************************************************/
//void smooth_set(q15_t FFT_Data[FREQ_RANGE][MAX_FFT_INDEX*2])
void smooth_set(q15_t FFT_Data[MAX_FFT_INDEX*2])
{
	//RECTIFICACION en el tiempo.
	//TODO AGREGAR AL modulo1 la inversa

	//FFT
	q15_t Envelope[4096];

	//FILTRO PASABAJO HANNING  filtrado con 1/2 ventana HANNING de 0.4s .
	//for(uint16_t i=0;i<FREQ_RANGE;i++)
	//{
	//FFT_conv_q15(FFT_Data[i],Envelope,TIME_DOMAIN);
	FFT_conv_q15(FFT_Data,Envelope,TIME_DOMAIN);
	//}

	//FINAL DE MODULO 2  - SUAVIZAR

	//INICIO DE MODULO 3 - DIFF_RECT
}
