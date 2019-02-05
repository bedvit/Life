#include "stdafx.h"
#include <iostream> 
#include "Grid.h"
#include "Calc.h"
#include <unordered_map>
//#include <concurrent_unordered_map.h>
#include <map>

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


Point Grid::GetCell(Point hDCPosition)
{
	Point res;
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


void Grid::Draw(HDC hDC, Point size)
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
	if (scale > 4)//�� ������ ����� ��� ������ <= 4 �������  
	{	
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
	}
	SelectObject(hDC, OldPen);
	DeleteObject(hPen);
	DeleteObject(OldPen);

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
void Grid::FillRectangle(HDC hDC, Calc &calc)
{
	//HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //����� �������� �����, ����������� ������ RGB - ������
	RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.
	std::unordered_map<LONGLONG, Point>::iterator i;

	for (i = calc.LifePoint.begin(); i != calc.LifePoint.end(); i++)
	{
		if (i->second.life) 
		{
			r.left = i->second.x * scale + position.x; //X-���������� �������� ������ ���� ��������������.
			r.top = i->second.y * scale + position.y; // i->second.y;//Y-���������� �������� ������ ���� ��������������.
			r.right = (i->second.x + 1) * scale + position.x;//X-���������� ������� ������� ���� ��������������.
			r.bottom = (i->second.y + 1) * scale + position.y; //Y-���������� ������� ������� ���� ��������������.

			//��������� �������������
			InvertRect(hDC, &r); //42
			//DrawFocusRect(hDC, &r); //��� 50% �������������� ������� //23
			//FillRect(hDC, &r, s); //��� 50% �������������� ������� //30
			//Rectangle(hDC, r.left, r.top, r.right, r.bottom); //27

			//����� ������
			//MoveToEx(hDC, r.left, r.top, NULL);  // ��� ����� 27
			//LineTo(hDC, r.right, r.bottom);

			//SetPixel(hDC, r.left, r.top, RGB(255, 51, 0)); //$$$SetPixel ������������� �������� ���� � ����� � ���������� ������������, GetPixel �������������� ���������� ���� //27

		}
	}
	//DeleteObject(s);
}


//void Grid::FillRectangle2(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBlackBrush, Calc &calc)
//{
//	//HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //����� �������� �����, ����������� ������ RGB - ������
//	RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.
//	std::unordered_map<LONGLONG, Point>::iterator i;
//
//	for (i = calc.LifePoint.begin(); i != calc.LifePoint.end(); i++)
//	{
//		if (i->second.life)
//		{
//			r.left = i->second.x * scale + position.x; //X-���������� �������� ������ ���� ��������������.
//			r.top = i->second.y * scale + position.y; // i->second.y;//Y-���������� �������� ������ ���� ��������������.
//			r.right = (i->second.x + 1) * scale + position.x;//X-���������� ������� ������� ���� ��������������.
//			r.bottom = (i->second.y + 1) * scale + position.y; //Y-���������� ������� ������� ���� ��������������.
//
//			pRT->BeginDraw();
//
//					pRT->DrawRectangle(
//						D2D1::RectF(
//							r.left,
//							r.top,
//							r.right,
//							r.bottom),
//						pBlackBrush);
//		pRT->EndDraw();
//		}
//	}
//	//DeleteObject(s);
//}
