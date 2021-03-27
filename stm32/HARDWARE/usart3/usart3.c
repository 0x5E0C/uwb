#include "usart3.h"

u8 USART3_RX_BUF[USART3_REC_LEN];
u8 USART3_byteNum;
u8 u3rx_flag=0;

void Gyro_USART3_Init(u32 bound)
{
    float temp;
    temp = 36000000 / bound;
    RCC->APB2ENR |= 1 << 3 ;
    RCC->APB1ENR |= 1 << 18;
    RCC->AHBENR |= 1 << 0;
    RCC->APB1RSTR |= 1 << 18;
    RCC->APB1RSTR &= ~(1 << 18);
    GPIOB->CRH &= 0xFFFF00FF;
    GPIOB->CRH |= 0x00008B00;
    USART3->BRR = (u16)temp;
    DMA1_Channel3->CNDTR = USART3_REC_LEN;
    DMA1_Channel3->CPAR = (u32)(&USART3->DR);
    DMA1_Channel3->CMAR = (u32)USART3_RX_BUF;
    DMA1_Channel3->CCR |= 0X3081;
    USART3->CR3 |= 1 << 6;
    MY_NVIC_Init(0, 1, USART3_IRQn, 2);
    USART3->CR1 = 0x201C;
}


void USART3_IRQHandler(void)
{
    if(USART3->SR & (1 << 3))
    {
        USART3->DR;
        USART3->SR &= ~(1 << 3);
    }
    if(USART3->SR & (1 << 4))
    {
		USART3->CR1 &= ~(1 << 5);
        USART3_byteNum = USART3->SR;
        USART3_byteNum = USART3->DR;
        USART3_byteNum = USART3_REC_LEN - DMA1_Channel3->CNDTR;
        DMA1_Channel3->CCR &= ~(1 << 0);
        DMA1_Channel3->CNDTR = USART3_REC_LEN;
		if(USART3_byteNum == 33)
		{
			u3rx_flag = 1;
			if(USART3_RX_BUF[1] == 0x51)
			{
				gyro.ax = ((short)(USART3_RX_BUF[3] << 8) | USART3_RX_BUF[2]) / 32768.0 * 16 * 9.8;
				gyro.ay = ((short)(USART3_RX_BUF[5] << 8) | USART3_RX_BUF[4]) / 32768.0 * 16 * 9.8;
				gyro.az = ((short)(USART3_RX_BUF[7] << 8) | USART3_RX_BUF[6]) / 32768.0 * 16 * 9.8;
			}
			if(USART3_RX_BUF[12] == 0x52)
			{
				gyro.wx = ((short)USART3_RX_BUF[14] << 8 | USART3_RX_BUF[13]) / 32768.0 * 2000;
				gyro.wy = ((short)USART3_RX_BUF[16] << 8 | USART3_RX_BUF[15]) / 32768.0 * 2000;
				gyro.wz = ((short)USART3_RX_BUF[18] << 8 | USART3_RX_BUF[17]) / 32768.0 * 2000;
			}
			if(USART3_RX_BUF[23] == 0x53)
			{
				gyro.roll = ((short)USART3_RX_BUF[25] << 8 | USART3_RX_BUF[24]) / 32768.0 * 180;
				gyro.pitch = ((short)USART3_RX_BUF[27] << 8 | USART3_RX_BUF[26]) / 32768.0 * 180;
				gyro.yaw = ((short)USART3_RX_BUF[29] << 8 | USART3_RX_BUF[28]) / 32768.0 * 180;
			}
		}
        DMA1_Channel3->CCR |= 1 << 0;
		USART3->CR1 |= 1<<5;
    }
}
