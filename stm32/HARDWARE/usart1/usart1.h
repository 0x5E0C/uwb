#ifndef __USART_H
#define __USART_H

#include "bsp.h"

#define USART1_REC_LEN 20

extern u8 USART1_RX_BUF[USART1_REC_LEN];
extern u8 USART1_byteNum;
extern u8 u1rx_flag;

void USART1_Send(u8 data);
void Wireless_USART1_Init(u32 bound);

#endif
















