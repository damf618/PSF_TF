#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "../inc/oversampling_filter.h"
#include "../inc/oversampling.h"

void oversampling( q15_t* down, uint16_t n_down, uint8_t over_factor, uint16_t fs)
{
	uint16_t 	sample = 0;
	int16_t 	adc   				   [ n_down*over_factor                ];
	int16_t 	oversampling_filtered  [ n_down*over_factor+OVER_LENGTH-1  ];

	uint16_t 	garbageOffset = (OVER_LENGTH + n_down*over_factor-1)/2-(n_down*over_factor)/2;

	q15_t 		down_sample[n_down];
	bool_t		data_acquired = FALSE;

	 while(!data_acquired)
	 {
		 adc[sample]          = ((int16_t )adcRead(CH1)-512)<<(6);

		 if ( ++sample>=n_down*over_factor )
		 {
			 gpioToggle ( LEDR );

			 //---------------filtrado antialias en digitial--------------------------
			 arm_conv_fast_q15  ( adc, n_down*over_factor,h,OVER_LENGTH,oversampling_filtered);

			 //---------------downsampling--------------------------
			 for(int i=0;i<n_down;i++)
			 {
				 down_sample[i]=oversampling_filtered[i*over_factor+garbageOffset];
			 }

			 data_acquired = TRUE;
			 sample = 0;
			 adcRead(CH1); 			//Requerido
		 }
		 gpioToggle ( LED1 );                                           // este led blinkea a fs/2

		 //---------------over sampling--------------------------
		 while(cyclesCounterRead()< EDU_CIAA_NXP_CLOCK_SPEED/(fs*over_factor));
	}
	memcpy(down,down_sample,sizeof(down_sample));
}

