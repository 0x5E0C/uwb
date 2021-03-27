#ifndef _USART2_H
#define _USART2_H

#include "bsp.h"

#define USART2_REC_LEN 50

extern u8 USART2_RX_BUF[USART2_REC_LEN];
extern u8 USART2_byteNum;
extern u8 u2rx_flag;

void USART2_Send(u8 data);
void UWB_USART2_Init(u32 bound);

#endif
