#pragma once
//#include "Point.h"
#include "Calc.h"
#include "Point.h"
//#include <d2d1.h>

class Grid
{
public:
	Grid();
	~Grid();

	Point position; // ������� ������ ��������� ����� ������������ ������ �������� ���� ���� � ���������
	long scalePoint; // ������� �������� �������� ���� ������
	//double scale; // ������� �������� �������� ���� ������

	void DrawLine(HDC hdc, Point size); // ���������� �����
	void Move(long x, long y); // �������� ���� �� x �������� ������ � y �����
	void AddScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void DecScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	//void FillRectangle(HDC hDC, Calc& calc, RECT& rect); //����������� �������������
	void Draw(unsigned char*& lpBitmapBits, Calc &calc, RECT& rect);
	
	Point GetCell(Point); // �������� ���������� ����� 
	//void DrawRect(HDC, HBRUSH, Point);



};


