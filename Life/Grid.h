#pragma once
//#include "Point.h"
#include "Calc.h"
#include "Point.h"
//#include <d2d1.h>

class Grid
{
public:

	Point position; // ������� ������ ��������� ����� ������������ ������ �������� ���� ���� � ���������
	long scale; // ������� �������� �������� ���� ������


	Grid();
	~Grid();

	void Draw(HDC hdc, Point size); // ���������� �����
	void Move(long x, long y); // �������� ���� �� x �������� ������ � y �����
	void AddScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void DecScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void FillRectangle(HDC hDC, Calc& calc, RECT& rect); //����������� �������������
	//void FillRectangle2(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBlackBrush, Calc& calc);
	
	Point GetCell(Point); // �������� ���������� ����� 
	//void DrawRect(HDC, HBRUSH, Point);



};


