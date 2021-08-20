#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "../inc/segmentation.h"
#include "../inc/freq_segment.h"
#include "../inc/hanning_filter.h"

extern header_struct_t header;

void init_cfft_instance(arm_cfft_instance_q15* CS,int length)//{{{
{
   switch(length){
      case 16:
         *CS=arm_cfft_sR_q15_len16;
         break;
      case 32:
         *CS=arm_cfft_sR_q15_len32;
         break;
      case 64:
         *CS=arm_cfft_sR_q15_len64;
         break;
      case 128:
         *CS=arm_cfft_sR_q15_len128;
         break;
      case 256:
         *CS=arm_cfft_sR_q15_len256;
         break;
      case 512:
         *CS=arm_cfft_sR_q15_len512;
         break;
      case 1024:
         *CS=arm_cfft_sR_q15_len1024;
         break;
      case 2048:
         *CS=arm_cfft_sR_q15_len2048;
         break;
      case 4096:
         *CS=arm_cfft_sR_q15_len4096;
         break;
      default:
         *CS=arm_cfft_sR_q15_len128;
   }
}


void conv_q15(q15_t* signal, q15_t* filter, uint32_t M )
{
	//Convolution result
	q15_t 	 signal_convolution  [ N_DOWNSAMPLING+M-1  ];
	uint16_t garbage_index = (M+N_DOWNSAMPLING-1)/2-N_DOWNSAMPLING/2;

	arm_conv_fast_q15(signal, N_DOWNSAMPLING,hanning,M,signal_convolution);

	//Extraccion de informacion util de la convolucion
	//TODO Podria realizarlo con un memcpy?
	for (uint16_t i =0;i<N_DOWNSAMPLING;i++)
	{
		signal[i] = signal_convolution[i+garbage_index];
	}
}

void cfft_q15(q15_t* signal, q15_t*samples)
{
	arm_cfft_instance_q15 CS;
	q15_t 		samples_fft[2*N_DOWNSAMPLING];

	for(uint16_t i =0;i<N_DOWNSAMPLING;i++)
	{
		samples_fft[i*2]=signal[i];
		samples_fft[i*2+1]=0;
	}
	init_cfft_instance ( &CS,N_DOWNSAMPLING        );
    arm_cfft_q15       ( &CS ,samples_fft ,0 ,1 );
    memcpy(samples,samples_fft,sizeof(samples_fft));
}

void cifft_q15(q15_t* signal_fft,q15_t* signal)
{
	arm_cfft_instance_q15 CS;

	init_cfft_instance ( &CS,N_DOWNSAMPLING        );
    arm_cfft_q15       ( &CS ,signal_fft ,1 ,1 );
    for(uint16_t i =0;i<N_DOWNSAMPLING;i++)
    {
    	signal[i]=signal_fft[i*2];
    }
}


void rfft_q15(q15_t* signal,uint8_t dir)
{
	arm_rfft_instance_q15 S;
	q15_t 		samples_fft[2*N_DOWNSAMPLING];
	arm_rfft_init_q15(&S,N_DOWNSAMPLING,dir,1); // inicializa una estructira que usa la funcion fft para procesar los datos. Notar el /2 para el largo
	arm_rfft_q15( &S,signal,samples_fft);
	arm_rfft_init_q15(&S,N_DOWNSAMPLING,1,1); // inicializa una estructira que usa la funcion fft para procesar los datos. Notar el /2 para el largo
	arm_rfft_q15( &S,samples_fft,signal);
}


void freq_segmentation( q15_t* samples,uint16_t* freq_range, uint16_t fs)
{
	/*---*** MODULO1:SEGMENTACION  ***---*/
	uint16_t 	cut_freq  = 0;
	uint16_t 	init_freq = 0;
	q15_t 		seg_fft[2*N_DOWNSAMPLING];
	float32_t	calc_aux  = 0;

	q15_t 		complex[2*N_DOWNSAMPLING];

	/*---*** MODULO2:ENVOLVENTE  ***---*/


	/*---*** MODULO3:DERIVACION & RECTIFICACION 1/2 ONDA  ***---*/
	q15_t derivative_filter[]={1,-1};

	/*** RFFT ***/
	/*arm_rfft_instance_q15 S;
	q15_t samples_fft[2*N_DOWNSAMPLING];
	arm_rfft_init_q15(&S,N_DOWNSAMPLING,0,1); // inicializa una estructira que usa la funcion fft para procesar los datos. Notar el /2 para el largo
	arm_rfft_q15( &S,samples,samples_fft);
	*/
	cfft_q15(samples,complex);

	/*** FILTRADO EN CORTE POR FRECUENCIAS ***/
/*
	//Eliminado de componente DC
	complex[0] = 0;
	complex[1] = 0;
*/
	//BORRADO EFICIENTE DE AMBOS EXTREMOS
	for(uint8_t j=1;j<N_FREQUENCIES;j++)
	{
		calc_aux  = (float32_t)((float32_t)fs/(float32_t)N_DOWNSAMPLING);
		calc_aux  = (float32_t)(1/calc_aux);
		cut_freq  = freq_range[j]*calc_aux;		  //Regla de 3 *considerasndo que son numeros complejos
		init_freq = freq_range[j-1]*calc_aux;	  //Regla de 3 *considerasndo que son numeros complejos
		for(uint16_t i =1;i<N_DOWNSAMPLING/2;i++)
		{
			if((init_freq<=i)&&(i<cut_freq))
			{
				seg_fft[i*2] 						= complex[i*2];							//Guardado frente parte real
				seg_fft[i*2+1] 						= complex[i*2+1];						//Guardado frente parte imaginaria
				seg_fft[(N_DOWNSAMPLING-1)*2-i*2] 	= complex[(N_DOWNSAMPLING-1)*2-i*2];	//Guardado fondo  parte real
				seg_fft[(N_DOWNSAMPLING-1)*2-i*2+1]	= complex[(N_DOWNSAMPLING-1)*2-i*2+1];	//Guardado fondo  parte imaginaria
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
		//arm_rfft_init_q15 ( &S,N_DOWNSAMPLING,1,1);
		//arm_rfft_q15	  ( &S,seg_fft, samples);	// Parte real de la transformada inversa de Fourier
		cifft_q15(seg_fft, samples);

		//TODO VALIDACION SEGMENTACION DE FRECUENCIAS
		/*	gpioToggle ( LEDR );
			for(uint16_t i = 0;i<N_DOWNSAMPLING;i++)
			{
				uartWriteByteArray ( UART_USB ,(uint8_t* )&samples[i], sizeof(samples[0]) );
			}
			gpioToggle ( LED1 );
			header.id++;
			uartWriteByteArray ( UART_USB ,(uint8_t*)&header ,sizeof(struct header_struct ));
		//}
		break;*/


		//PROCESAMIENTO CONTINUO! DE LO CONTRARIO SE SATURA LA MEMORIA NO ES POSIBLE GENERAR UN VECTOR

		/***----*** MOD 2 ***----***/
		/*      SMOOOTHING    */

		//RECTIFICACION DE ONDA COMPLETA PARA NUMEROS_Q !
		for (uint16_t i =0;i<N_DOWNSAMPLING;i++)
		{
			//El negativo en numeros Q es el bit 16
			if(samples[i]<0)
			{
				samples[i]=-1*samples[i];
			}
		}

		//TODO VALIDACION RECTIFICACION
		/*gpioToggle ( LEDR );
		for(uint16_t i = 0;i<N_DOWNSAMPLING;i++)
		{
			uartWriteByteArray ( UART_USB ,(uint8_t* )&samples[i], sizeof(samples[0]) );
		}
		gpioToggle ( LED1 );
		header.id++;
		uartWriteByteArray ( UART_USB ,(uint8_t*)&header ,sizeof(struct header_struct ));
		//}
	break;*/

		//CONVOLUCION CON FILTRO 1/2 VENTANA HANNING
		conv_q15(samples,hanning,HANNING_LENGTH);

		//TODO VALIDACION FILTRO_HANNING
		/*gpioToggle ( LEDR );
		for(uint16_t i = 0;i<N_DOWNSAMPLING;i++)
		{
			uartWriteByteArray ( UART_USB ,(uint8_t* )&samples[i], sizeof(samples[0]) );
		}
		gpioToggle ( LED1 );
		header.id++;
		uartWriteByteArray ( UART_USB ,(uint8_t*)&header ,sizeof(struct header_struct ));
		//}
	break;*/

		/***----*** MOD 3 ***----***/
		/*      DERIVATIVE HALF-RECT   */

		//DERIVACION
		conv_q15(samples,derivative_filter,2);

		//TODO VALIDACION DERIVADA
		/*gpioToggle ( LEDR );
		for(uint16_t i = 0;i<N_DOWNSAMPLING;i++)
		{
			uartWriteByteArray ( UART_USB ,(uint8_t* )&samples[i], sizeof(samples[0]) );
		}
		gpioToggle ( LED1 );
		header.id++;
		uartWriteByteArray ( UART_USB ,(uint8_t*)&header ,sizeof(struct header_struct ));
		//}
	break;*/

		//RECTIFICACION DE 1/2 ONDA
		for (uint16_t i =0;i<N_DOWNSAMPLING;i++)
		{
			//El negativo en numeros Q es el bit 16
			if(samples[i]<0)
			{
				samples[i]=0;
			}
		}
		//TODO VALIDACION DERIVADA

		gpioToggle ( LEDR );
		for(uint16_t i = 0;i<N_DOWNSAMPLING;i++)
		{
			uartWriteByteArray ( UART_USB ,(uint8_t* )&samples[i], sizeof(samples[0]) );
		}
		gpioToggle ( LED1 );
		header.id++;
		uartWriteByteArray ( UART_USB ,(uint8_t*)&header ,sizeof(struct header_struct ));
		//}
	break;

		/***----*** MOD 4 ***----***/
		/*      COMB FILTER     */
	}
}

