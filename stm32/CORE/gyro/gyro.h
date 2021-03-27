#ifndef _GYRO_H
#define _GYRO_H

#include "bsp.h"

typedef struct _GYRO
{
	float ax;
	float ay;
	float az;
	float wx;
	float wy;
	float wz;
	float roll;
	float pitch;
	float yaw;
} Gyro;

extern Gyro gyro;

float GetAngleBias(float target);

#endif
