#pragma once
//#include "Point.h"
#include "Calc.h"

class Grid
{
public:

	POINT position; // ������� ������ ��������� ����� ������������ ������ �������� ���� ���� � ���������
	long scale; // ������� �������� �������� ���� ������


	Grid();
	~Grid();

	void Draw(HDC hdc, POINT size); // ���������� �����
	void Move(long x, long y); // �������� ���� �� x �������� ������ � y �����
	void AddScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void DecScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void FillRectangle(HDC hDC, Calc calc); //����������� �������������
	
	POINT GetCell(POINT); // �������� ���������� ����� 
	void DrawRect(HDC, HBRUSH, POINT);



};


