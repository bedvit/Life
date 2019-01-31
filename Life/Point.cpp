#include "stdafx.h"
#include "Point.h"


Point::Point()
{
	x=0;
	y=0;
	state = 0;
	life = false;
	update = 0;
}

Point::Point(long x, long y)
{
	this->x = x;
	this->y = y;
	this->state = 0;
	this->life = false;
	this->update = 0;
}

Point::Point(long x,long y,char state)
{
	this->x = x;
	this->y = y;
	this->state = state;
	this->life = false;
	this->update = 0;
}

Point::Point(long x, long y, char state, bool life)
{
	this->x = x;
	this->y = y;
	this->state = state;
	this->life = life;
	this->update = 0;
}

Point::Point(long x, long y, char state, bool life, char update)
{
	this->x = x;
	this->y = y;
	this->state = state;
	this->life = life;
	this->update = update;
}

Point::~Point()
{
}
