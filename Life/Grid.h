#pragma once
//#include "Point.h"
//#include "Calc.h"
#include "Point.h"
#include <unordered_map>

class Grid
{
public:
	Grid();
	~Grid();

	Point position; // ������� ������ ��������� ����� ������������ ������ �������� ���� ���� � ���������
	long scalePoint; // ������� �������� �������� ���� ������
	void DrawLine(HDC hdc, Point size); // ���������� �����
	void Move(long x, long y); // �������� ���� �� x �������� ������ � y �����
	void AddScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void DecScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void Draw(unsigned char*& lpBitmapBits, std::unordered_map <LONGLONG, Point>& LifePoint, RECT& rect);
	Point GetCell(Point); // �������� ���������� ����� 
	bool updateBuffer;
	void DrawPoint(Point &point);
};


