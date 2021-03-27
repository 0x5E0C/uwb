#include "bsp.h"

int main()
{
	Stm32_Clock_Init(9);
    delay_init(72);
    BSP_Init();
	PID_Total_Init();
	KalMan_Total_Init();
	startLocate();
	Stop();
	roominfo.lenth=300;
	roominfo.width=300;
    while(1)
	{
		if(u2rx_flag)
		{
			UpdateDisinfo();
			SendPos();
		}
	}
}

 




