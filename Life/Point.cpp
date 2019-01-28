#include "stdafx.h"
#include "Point.h"


Point::Point()
{
	x=0;
	y=0;
	state = 0;
}

Point::Point(long x, long y)
{
	this->x = x;
	this->y = y;
	this->state = 0;
}

Point::Point(long x,long y,char state)
{
	this->x = x;
	this->y = y;
	this->state = state;
}


Point::~Point()
{
}
