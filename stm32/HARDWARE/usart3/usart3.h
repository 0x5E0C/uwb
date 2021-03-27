#ifndef _USART3_H
#define _USART3_H

#include "bsp.h"

#define USART3_REC_LEN 66

extern u8 USART3_RX_BUF[USART3_REC_LEN];
extern u8 USART3_byteNum;
extern u8 u3rx_flag;

void Gyro_USART3_Init(u32 bound);

#endif
