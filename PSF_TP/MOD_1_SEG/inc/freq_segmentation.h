
#ifndef FREQ_SEGMENT
#define FREQ_SEGMENT

#include "arm_math.h"

/***************************************************/
/*
 *          DEFINE Segmentacion de Frecuencia
 */
/***************************************************/
#define F0 0			//Rango 1 Instrumentos Baja          Freq
#define F1 200			//Rango 1 Instrumentos Baja          Freq
#define F2 400			//Rango 2 Instrumentos Baja / Media  Freq
#define F3 800			//Rango 3 Instrumentos Media         Freq
#define F4 1600			//Rango 4 Instrumentos Alta / Media  Freq
#define F5 3200			//Rango 5 Instrumentos Alta          Freq

#define MAX_SAMPLES 1024
#define FREQ_RANGE 6
#define RANGE_LENGHT MAX_SAMPLES/FREQ_RANGE

/***************************************************/
/*
 *          Estructuras y Tipos de Variables
 */
/***************************************************/
typedef struct segmentation_struct {
   q15_t FFT_Range[FREQ_RANGE][RANGE_LENGHT];
   uint16_t fs;
   uint16_t N;
} seg_struct_t;

/***************************************************/
/*
 *                 Variables
 */
/***************************************************/
seg_struct_t segmentation;


/***************************************************/
/*
 *          Prototipos de Funciones
 */
/***************************************************/
uint8_t seg_init(uint16_t Freq_S, uint16_t N_data);
void seg_set(q15_t* FFT);

#endif
