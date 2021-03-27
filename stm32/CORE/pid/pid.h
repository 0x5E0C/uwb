#ifndef _PID_H
#define _PID_H

#include "bsp.h"

#define No_Max	FLT_MAX
#define No_Min  -FLT_MAX

typedef struct _PID 
{
	float SetValue;
	float ActualValue;
	float Err;
	float Last_Err;
	float Last_Err2;
	float Err_Sum;
	float P,I,D;
	float OutputMax;
	float OutputMin;
	float OutputPoint;
} PID;

extern PID PID_Gyro;

void PID_Total_Init(void);
void PID_Init(PID *pid);
float Incremental_PID(PID *pid);
float Positional_PID(PID *pid);

#endif
