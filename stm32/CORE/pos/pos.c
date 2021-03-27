#include "pos.h"

Roominfo roominfo;
Posinfo posinfo;
Distance distance;

void startLocate()
{
	u8 i;
	u8 buffer[11]={0x01,0x10,0x00,0x28,0x00,0x01,0x02,0x00,0x04,0xA1,0xBB};
	for(i=0;i<=11;i++)
	{
		USART2_Send(buffer[i]);
	}
	while(!u2rx_flag);
	delay_ms(1000);
	UpdateDisinfo();
}

void SendPos()
{
	u8 buffer[PACKET_LENGTH];
	u16 sum=0;
	u8 i;
	buffer[0]=0x5E;
	buffer[1]=0x0C;
	buffer[2]=0x01;
	buffer[3]=SENDPOS_CMD;
	buffer[4]=(posinfo.x&0xFF00)>>8;
	buffer[5]=posinfo.x&0x00FF;
	buffer[6]=(posinfo.y&0xFF00)>>8;
	buffer[7]=posinfo.y&0x00FF;
	for(i=0;i<=7;i++)
	{
		sum+=buffer[i];
		USART1_Send(buffer[i]);
	}
	USART1_Send((sum&0xFF00)>>8);
	USART1_Send(sum&0x00FF);
}

void UpdateDisinfo()
{
	u2rx_flag=0;
	if(USART2_byteNum==31)
	{
		distance.a=USART2_RX_BUF[15]<<8|USART2_RX_BUF[16];
		distance.b=USART2_RX_BUF[17]<<8|USART2_RX_BUF[18];
		distance.c=USART2_RX_BUF[19]<<8|USART2_RX_BUF[20];
		distance.d=USART2_RX_BUF[21]<<8|USART2_RX_BUF[22];
		CalPosInfo();
	}
}

void CalPosInfo()
{
	distance.a=(short)KalmanFilter(&ADistance,(double)distance.a);
	distance.b=(short)KalmanFilter(&BDistance,(double)distance.b);
	distance.c=(short)KalmanFilter(&BDistance,(double)distance.c);
	distance.d=(short)KalmanFilter(&DDistance,(double)distance.d);
	posinfo=Average(Calpos1(distance),Calpos2(distance),Calpos3(distance),Calpos4(distance));
}

Posinfo Average(Posinfo p1,Posinfo p2,Posinfo p3,Posinfo p4)
{
	Posinfo result;
	result.x=(p1.x+p2.x+p3.x+p4.x)/4;
	result.y=(p1.y+p2.y+p3.y+p4.y)/4;
	return result;
}

Posinfo Calpos1(Distance dis)
{
#if CHAN_ALGORITHM
	return CalPosByChan(0,0,distance.a,0,roominfo.width,distance.b,roominfo.lenth,roominfo.width,distance.c);
#else
	Posinfo result;
	int l=roominfo.lenth;
	float x,y;
	int a=dis.a;
	int d=dis.d;
	x=(a*a+l*l-d*d)/(2*l);
	y=pow((a*a-x*x),0.5);
	result.x=x;
	result.y=y;
	return result;
#endif
}

Posinfo Calpos2(Distance dis)
{
#if CHAN_ALGORITHM
	return CalPosByChan(0,0,distance.a,0,roominfo.width,distance.b,roominfo.lenth,0,distance.d);
#else
	Posinfo result;
	int w=roominfo.width;
	float x,y;
	int a=dis.a;
	int b=dis.b;
	y=(a*a+w*w-b*b)/(2*w);
	x=pow((a*a-y*y),0.5);
	result.x=x;
	result.y=y;
	return result;
#endif
}

Posinfo Calpos3(Distance dis)
{
#if CHAN_ALGORITHM
	return CalPosByChan(0,0,distance.a,roominfo.lenth,roominfo.width,distance.c,roominfo.lenth,0,distance.d);
#else
	Posinfo result;
	int l=roominfo.lenth;
	int w=roominfo.width;
	float x,y;
	int b=dis.b;
	int c=dis.c;
	x=(b*b+l*l-c*c)/(2*l);
	y=w-pow((b*b-x*x),0.5);
	result.x=x;
	result.y=y;
	return result;
#endif
}

Posinfo Calpos4(Distance dis)
{
#if CHAN_ALGORITHM
	return CalPosByChan(0,roominfo.width,distance.b,roominfo.lenth,roominfo.width,distance.c,roominfo.lenth,0,distance.d);
#else
	Posinfo result;
	int l=roominfo.lenth;
	int w=roominfo.width;
	float x,y;
	int c=dis.c;
	int d=dis.d;
	y=(d*d+w*w-c*c)/(2*w);
	x=l-pow((d*d-y*y),0.5);
	result.x=x;
	result.y=y;
	return result;
#endif
}

#if CHAN_ALGORITHM
Posinfo CalPosByChan(double x1,double y1,double r1,double x2,double y2,double r2,double x3,double y3,double r3)
{
	double x21=x2-x1;
	double x31=x3-x1;
	double y21=y2-y1;
	double y31=y3-y1;
	double r31=r3-r1;
	double r21=r2-r1;
	double K1=x1*x1+y1*y1;
	double K2=x2*x2+y2*y2;
	double K3=x3*x3+y3*y3;
	double p1=(y21*r31*r31-y31*r21*r21+y31*(K2-K1)-y21*(K3-K1))/(2*(x21*y31-x31*y21));
	double p2=(x21*r31*r31-x31*r21*r21+x31*(K2-K1)-x21*(K3-K1))/(2*(x31*y21-x21*y31));
	double q1=(y21*r31-y31*r21)/(x21*y31-x31*y21);
	double q2=(x21*r31-x31*r21)/(x31*y21-x21*y31);
	double x=p1+q1*r1;
	double y=p2+q2*r1;
	Posinfo result;
	result.x=x;
	result.y=y;
	return result;
}
#endif
