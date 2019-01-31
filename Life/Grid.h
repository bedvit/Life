#pragma once
//#include "Point.h"
#include "Calc.h"
#include "Point.h"

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
	void FillRectangle(HDC hDC, Calc& calc); //����������� �������������
	
	Point GetCell(Point); // �������� ���������� ����� 
	void DrawRect(HDC, HBRUSH, Point);



};


