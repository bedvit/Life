#pragma once
class Point
{
public:
	Point();
	~Point();
	Point(long x, long y);
	Point(long x, long y, char state);
	Point(long x, long y, char state, bool life);
	Point(long x, long y, char state, bool life, char update);
	long x;
	long y;
	char state;
	bool life;
	char update;
};

