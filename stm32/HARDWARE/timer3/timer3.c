#include "timer3.h"

void Timing_TIM3_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= 1 << 1;
    TIM3->ARR = arr-1;
    TIM3->PSC = psc-1;
    TIM3->DIER |= 1 << 0;
    MY_NVIC_Init(1, 1, TIM3_IRQn, 2);
    TIM3->CR1 |= 0x01;
}

void TIM3_IRQHandler(void)
{
	int bias;
    if(TIM3->SR & 0X0001)
    {
		if(!stop_flag)
		{
			PID_Gyro.ActualValue=GetAngleBias(car_attitude.angle);
			bias=Incremental_PID(&PID_Gyro);
			if(abs(bias)<500)
			{
				PID_Init(&PID_Gyro);			
			}
			SetSpeed(car_attitude.speed+bias,car_attitude.speed-bias);
		}
    }
    TIM3->SR &= ~(1 << 0);
}
