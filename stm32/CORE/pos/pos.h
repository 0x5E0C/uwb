#ifndef _POS_H
#define _POS_H

#include "bsp.h"

#define CHAN_ALGORITHM 0

typedef struct _ROOMINFO
{
	int lenth;
	int width;
} Roominfo;

typedef struct _POSINFO
{
	int x;
	int y;
} Posinfo;

typedef struct _DISTANCE
{
	short a;
	short b;
	short c;
	short d;
} Distance;

extern Roominfo roominfo;
extern Posinfo posinfo;
extern Distance distance;

void startLocate(void);
void SendPos(void);
void UpdateDisinfo(void);
void CalPosInfo(void);
Posinfo Average(Posinfo p1,Posinfo p2,Posinfo p3,Posinfo p4);
Posinfo Calpos1(Distance dis);
Posinfo Calpos2(Distance dis);
Posinfo Calpos3(Distance dis);
Posinfo Calpos4(Distance dis);
#if CHAN_ALGORITHM
Posinfo CalPosByChan(double x1,double y1,double r1,double x2,double y2,double r2,double x3,double y3,double r3);
#endif

#endif
