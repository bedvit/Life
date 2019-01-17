#include "stdafx.h"
#include <iostream> 
#include "Grid.h"
#include "Calc.h"
#include <unordered_map>

Grid::Grid()
{
	position.x = 0;
	position.y = 0;
	scale = 15; // ��-��������� ������ ������ 15 ��������

}

Grid::~Grid()
{
}


void Grid::Move(long x, long y)
{
	position.x += x;
	position.y += y;
}


POINT Grid::GetCell(POINT hDCPosition)
{
	POINT res;
	long xTmp = (hDCPosition.x - position.x);
	long yTmp = (hDCPosition.y - position.y);
	if (xTmp < 0) //��� ������������� ����������� ���������� � ������� ������� (������������� ������� ������ ����������� ������� �����)
		res.x = xTmp / scale - 1;
	else
		res.x = xTmp / scale;

	if (yTmp < 0)
		res.y = yTmp / scale - 1;
	else
		res.y = yTmp / scale;

	//res.x = (hDCPosition.x - position.x) / scale; // ���������� ���� � �����
	//res.y = (hDCPosition.y - position.y) / scale;

	return res;
}


void Grid::Draw(HDC hDC, POINT size)
{
	HPEN hPen, OldPen; //����������� ����

	long startFade = 12;
	long grayScale = 222;

	if (scale > 4 && scale < startFade) //������ ����������� ������� �����
	{
		grayScale = 222 + (startFade - scale) * 32 / (startFade - 5);
	}

	hPen = CreatePen(PS_SOLID, 1, RGB(grayScale, grayScale, grayScale)); //������ ���� ������� �����
	OldPen = (HPEN)SelectObject(hDC, hPen); //������ �������� �������
	   	 
	long px = position.x % scale; // x ���������� ������ ������������ ����� 
	long py = position.y % scale; // y ���������� ������ ������������ ����� 

	long wx = size.x / scale + 1; // ���������� ����� �� �����������, ������� � ����
	long wy = size.y / scale + 1; // ���������� ����� �� ���������, ������� � ����
	// ������� �����
	if (scale <= 4) return; //�� ������ ����� ��� ������ <= 4 �������  
	// ������� ������������ �����
	for (long x = 0; x < wx; x++)
	{
		//SetPixel(hDC, px + x * scale, 0, RGB(255, 51, 0)); //$$$SetPixel ������������� �������� ���� � ����� � ���������� ������������, GetPixel �������������� ���������� ����
		MoveToEx(hDC, px + x * scale, 0, NULL); // �������� �����
		LineTo(hDC, px + x * scale, size.y); // �� �� �������� �����
	}

	// ����� ��������������
	for (long y = 0; y < wy; y++)
	{
		MoveToEx(hDC, 0, py + y * scale, NULL);
		LineTo(hDC, size.x, py + y * scale);
	}

	SelectObject(hDC, OldPen);
	DeleteObject(hPen);

	//MoveToEx(hDC, 0, 0, NULL);  // ��� �����
	//LineTo(hDC, size.x, size.y);

}

void Grid::AddScale(long x, long y) // ��������� ������� ������������ �� ����� x, y
{
	if (scale == 50) return; // ������������ ������� = 50 ��������
	
	float zx = ((float)(x - position.x)) / scale; // ���������� ���� � �����
	float zy = ((float)(y - position.y)) / scale;

	scale++;

	position.x = x - zx * scale; // ����� ���������� ������ �����, ����� ��� �������� ������ �� ��������� �� ����� ���������������
	position.y = y - zy * scale;

}

void Grid::DecScale(long x, long y)
{
	if (scale == 1) return; // ����������� ������� = 1 �������

	float zx = ((float)(x - position.x)) / scale; // ���������� ���� � �����
	float zy = ((float)(y - position.y)) / scale;

	scale--;

	position.x = x - zx * scale; // ����� ���������� ������ ����� 
	position.y = y - zy * scale;
}
void Grid::FillRectangle(HDC hDC, Calc calc)
{
	HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //����� �������� �����, ����������� ������ RGB - ������

	std::unordered_map<LONGLONG, POINT>::iterator i;

	for (i = calc.LifePoint.begin(); i != calc.LifePoint.end(); i++)
	{
		DrawRect(hDC, s, i->second);
	}

	DeleteObject(s);
}

void Grid::DrawRect(HDC hDC, HBRUSH s, POINT pos)
{

	RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.
	r.left = pos.x * scale + position.x; //X-���������� �������� ������ ���� ��������������.
	r.top = pos.y * scale + position.y; // i->second.y;//Y-���������� �������� ������ ���� ��������������.
	r.right = (pos.x + 1) * scale + position.x;//X-���������� ������� ������� ���� ��������������.
	r.bottom = (pos.y + 1) * scale + position.y; //Y-���������� ������� ������� ���� ��������������.

	//��������� �������������
	FillRect(hDC, &r, s);

}

