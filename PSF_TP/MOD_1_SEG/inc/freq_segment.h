
#ifndef FREQ_SEG
#define FREQ_SEG

/***************************************************/
/*
 *           CONFIGURACION MODULO 0
 *                OVERSAMPLING
 */
/***************************************************/
/* La cantidad de datos a guardar para generar el oversampling
 * sera con un oversample de 2
 * */
#define N_OVERSAMPLE   		2

/***************************************************/
/*
 *           CONFIGURACION MODULO 1
 *                OVERSAMPLING
 */
/***************************************************/

/*Minimo de ciclos a considerar es 2.2 ciclos, deseamos acotar
el numero de muestras a 2048, por lo que obtenemos que:
     N = fs * t  ->    fs = 2048 / (2.2/3)
*/
#define CYCLES 				22
#define K_CONSTANT			10
#define BPM_TO_HZ			60
#define MIN_BPM				180
#define N_DOWNSAMPLING		2048

//fs = N_DOWNSAMPLING /CYCLES/K_CONSTANT / MIN_BPM / BPM_TO_HZ


/***************************************************/
/*
 *          DEFINE Segmentacion de Frecuencia
 */
/***************************************************/
#define F0					0
#define F1					200						//Rango 1 Instrumentos Baja          Freq
#define F2					400						//Rango 2 Instrumentos Baja / Media  Freq
#define F3					800						//Rango 3 Instrumentos Media         Freq
#define F4 					1600					//Rango 4 Instrumentos Alta / Media  Freq
#define	N_FREQUENCIES		5

#endif	//OVER_SAMPLING
