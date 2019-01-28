#pragma once
class Point
{
public:
	Point();
	Point( long x, long y);
	Point( long x, long y, char state);
	~Point();
	long x;
	long y;
	char state;
};

