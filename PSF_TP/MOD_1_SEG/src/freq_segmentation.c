#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "../inc/freq_segmentation.h"

extern seg_struct_t segmentation;
uint16_t Freq_Ranges[FREQ_RANGE]={F0,F1,F2,F3,F4,F5};

/***************************************************/
uint8_t seg_init(uint16_t Freq_S, uint16_t N_data)
{
	segmentation.fs = Freq_S;
	segmentation.N = N_data;
}


void seg_set(q15_t* FFT)
{
	uint16_t f_range = segmentation.N/segmentation.fs;			//Freq_X = fs/N * index
	uint16_t index = 0;
	uint16_t n_freq_ranges = sizeof(Freq_Ranges)/sizeof(Freq_Ranges[0]);  //Nro de Frecuencias definidas

	for (uint16_t i=0;i<n_freq_ranges-1;i++)				//Numero de rangos de frecuecnias definidas
	{
		Freq_Ranges[i];											//Rango minimo
		Freq_Ranges[i+1];										//Rango maximo

		//indice minimo y maximo de las frecuencias minimo y maximo.
		for(uint16_t j=((f_range)*(Freq_Ranges[i]));j<=((f_range)*(Freq_Ranges[i+1]));j++)
		{
			//Almacenamiento de Rango de FFT Correspondiente al rango
			segmentation.FFT_Range[i][index] = FFT[j];
			index++;
		}
	}

}

