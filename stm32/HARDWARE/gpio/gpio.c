#include "gpio.h"

void GPIO_Init()
{
	RCC->APB2ENR|=1<<3;
	
	GPIOB->CRL&=0XFFFFFF00;
	GPIOB->CRL|=0X00000033;
}
