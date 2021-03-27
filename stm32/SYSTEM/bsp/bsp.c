#include "bsp.h"

int fputc(int ch, FILE* f)
{
    while((USART1->SR & 0X40) == 0);
    USART1->DR = (u8) ch;
    return ch;
}

void BSP_Init()
{
	GPIO_Init();
    Wireless_USART1_Init(9600);
    UWB_USART2_Init(115200);
    Gyro_USART3_Init(115200);
    Motor_TIM2_Init(10000,72);
    Timing_TIM3_Init(20000,72);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}

