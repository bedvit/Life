#pragma once



#ifdef _WIN64
#define SIZE_POINT 9
#define SIZE_LIFEPOINT 33000000 //MAX ���������� ����� � ���� � ������ ������
#elif _WIN32
#define SIZE_POINT 5
#define SIZE_LIFEPOINT 22000000 //MAX ���������� ����� � ���� � ������ ������
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

