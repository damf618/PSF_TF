
#ifndef OVER_SAMPLING
#define OVER_SAMPLING

#include "arm_math.h"
#include "sapi.h"


/***************************************************/
/*
 *          Prototipos de Funciones
 */
/***************************************************/
void oversampling( q15_t* down, uint16_t n_down, uint8_t over_factor, uint16_t fs);

#endif	//OVER_SAMPLING
