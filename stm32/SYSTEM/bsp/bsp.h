#ifndef _BSP_H
#define _BSP_H


#define PACKET_LENGTH  10
#define BASE_SPEED	   8000

#define SENDPOS_CMD	0x05

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>
#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "timer2.h"
#include "timer3.h"
#include "pid.h"
#include "motor.h"
#include "gyro.h"
#include "pos.h"
#include "kalman.h"	

void BSP_Init(void);
void PID_Total_Init(void);

#endif
