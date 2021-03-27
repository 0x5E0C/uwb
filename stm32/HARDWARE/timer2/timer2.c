#include "timer2.h"

void Motor_TIM2_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR|=1<<0; 
	RCC->APB2ENR|=1<<3;	
	GPIOA->CRL&=0XFFFFFF00;
	GPIOA->CRL|=0X000000BB;

	TIM2->ARR=arr-1; 
	TIM2->PSC=psc-1;
	
	TIM2->CCMR1|=6<<4;
	TIM2->CCMR1|=1<<3;
	TIM2->CCER|=1<<0;
	
	TIM2->CCMR1|=6<<12;
	TIM2->CCMR1|=1<<11;
	TIM2->CCER|=1<<4;
	
	TIM2->CR1=0x0080;
	TIM2->CR1|=0x01;
}


