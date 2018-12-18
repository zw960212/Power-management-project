#ifndef __CMD_COMM__H
#define	__CMD_COMM__H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ---------------------------------------------------------------*/
#include "stdint.h"

/* Exported functions ------------------------------------------------------- */
void DataCompress(uint8_t* src,uint16_t srcLen,uint8_t *dest);


#ifdef __cplusplus
}
#endif

#endif		// #ifndef __UART_COMM__H


