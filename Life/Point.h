#pragma once

#ifdef _WIN64
#define SIZE_POINT 9
#elif _WIN32
#define SIZE_POINT 5
#endif

class Point
{
public:
	Point();
	~Point();

	Point(long x, long y);
	long x;
	long y;
};

