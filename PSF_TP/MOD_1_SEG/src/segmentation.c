#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "../inc/segmentation.h"
#include "../inc/freq_segment.h"

void freq_segmentation( q15_t* samples, q15_t seg_signlas[N_FREQUENCIES-1][N_DOWNSAMPLING],uint16_t* freq_range, uint16_t fs)
{
	uint16_t 	cut_freq  = 0;
	uint16_t 	init_freq = 0;
	q15_t 		seg_fft[2*N_DOWNSAMPLING];

	/*** RFFT ***/
	arm_rfft_instance_q15 S;
	q15_t 		samples_fft[2*N_DOWNSAMPLING];

	arm_rfft_init_q15 ( &S,N_DOWNSAMPLING,0,1);
	arm_rfft_q15	  ( &S,samples, samples_fft);

	/*** FILTRADO EN CORTE POR FRECUENCIAS ***/
	//Eliminado de componente DC
	samples_fft[0] = 0;

	//BORRADO EFICIENTE DE AMBOS EXTREMOS
	for(uint8_t j=1;j<N_FREQUENCIES;j++)
	{
		cut_freq  = freq_range[j]/(fs/N_DOWNSAMPLING);		  //Regla de 3 *considerasndo que son numeros complejos
		init_freq = freq_range[j-1]/(fs/N_DOWNSAMPLING);		  //Regla de 3 *considerasndo que son numeros complejos
		for(uint16_t i =0;i<N_DOWNSAMPLING;i++)
		{
			if((init_freq<=i)&&(i<cut_freq))
			{
				seg_fft[i*2] 						= samples_fft[i*2];							//Guardado frente parte real
				seg_fft[i*2+1] 						= samples_fft[i*2+1];						//Guardado frente parte imaginaria
				seg_fft[(N_DOWNSAMPLING-1)*2-i*2] 	= samples_fft[(N_DOWNSAMPLING-1)*2-i*2];	//Guardado fondo  parte real
				seg_fft[(N_DOWNSAMPLING-1)*2-i*2+1]	= samples_fft[(N_DOWNSAMPLING-1)*2-i*2+1];	//Guardado fondo  parte imaginaria
			}
			else
			{
				seg_fft[i*2] 						= 0;  //Borrado frente parte real
				seg_fft[i*2+1] 						= 0;  //Borrado frente parte imaginaria
				seg_fft[(N_DOWNSAMPLING-1)*2-i*2] 	= 0;  //Borrado fondo  parte real
				seg_fft[(N_DOWNSAMPLING-1)*2-i*2+1]	= 0;  //Borrado fondo  parte imaginaria
			}
		}
		/*** IFFT ***/
		arm_rfft_init_q15 ( &S,N_DOWNSAMPLING,1,1);
		arm_rfft_q15	  ( &S,seg_fft, seg_signlas[j]);	// Parte real de la transformada inversa de Fourier
	}
}

