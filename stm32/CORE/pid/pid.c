#include "pid.h"

PID PID_Gyro;

void PID_Total_Init()
{
	PID_Gyro.P = 47;
    PID_Gyro.I = 0.19;
    PID_Gyro.D = 0; 
    PID_Gyro.OutputMax = 10000;
    PID_Gyro.OutputMin = -10000;
    PID_Init(&PID_Gyro);
}

void PID_Init(PID* pid)
{
    pid->ActualValue = 0;
    pid->Err = 0;
    pid->Last_Err = 0;
    pid->Last_Err2 = 0;
}

float Incremental_PID(PID* pid)
{
    pid->Err = pid->ActualValue - pid->SetValue;
    pid->OutputPoint += pid->P * (pid->Err - pid->Last_Err) +
                        pid->I * pid->Err +
                        pid->D * (pid->Err - 2 * pid->Last_Err + pid->Last_Err2);
    pid->Last_Err2 = pid->Last_Err;
    pid->Last_Err = pid->Err;
    if(pid->OutputPoint > pid->OutputMax)
    {
        pid->OutputPoint = pid->OutputMax;
    }
    else if(pid->OutputPoint < pid->OutputMin)
    {
        pid->OutputPoint = pid->OutputMin;
    }
    return pid->OutputPoint;
}

float Positional_PID(PID* pid)
{
    pid->Err = pid->ActualValue - pid->SetValue;
    pid->Err_Sum += pid->Err;
    pid->OutputPoint += pid->P * pid->Err +
                        pid->I * pid->Err_Sum +
                        pid->D * (pid->Err - pid->Last_Err);
    pid->Last_Err = pid->Err;
    if(pid->OutputPoint > pid->OutputMax)
    {
        pid->OutputPoint = pid->OutputMax;
    }
    else if(pid->OutputPoint < pid->OutputMin)
    {
        pid->OutputPoint = pid->OutputMin;
    }
    return pid->OutputPoint;
}
