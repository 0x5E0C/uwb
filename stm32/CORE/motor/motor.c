#include "motor.h"

bool stop_flag=false;
Attitude car_attitude;

void SetSpeed(int left_speed,int right_speed)
{
	stop_flag=false;
	if(left_speed<0)
	{
		TIM2->CCR1=10000+left_speed;
		PBout(0)=1;
	}
	else
	{
		TIM2->CCR1=left_speed;
		PBout(0)=0;
	}
	if(right_speed<0)
	{
		TIM2->CCR2=10000+right_speed;
		PBout(1)=1;
	}
	else
	{
		TIM2->CCR2=right_speed;
		PBout(1)=0;
	}
}

void Move(int speed,float angle)
{
	stop_flag=false;
	car_attitude.speed=speed;
	car_attitude.angle=angle;
	PID_Gyro.SetValue=0;
}

void Stop()
{
	stop_flag=true;
	TIM2->CCR1=0;
	TIM2->CCR2=0;
	PBout(0)=0;
	PBout(1)=0;
	PID_Init(&PID_Gyro);
}

