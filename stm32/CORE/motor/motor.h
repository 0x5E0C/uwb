#ifndef _MOTOR_H
#define _MOTOR_H

#include "bsp.h"

typedef struct _Attitude 
{
	int speed;
	int w;
	float angle;
} Attitude;

extern bool stop_flag;
extern Attitude car_attitude;

void SetSpeed(int left_speed,int right_speed);
void Move(int speed,float angle);
void Stop(void);

#endif
