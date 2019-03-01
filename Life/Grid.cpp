#include "stdafx.h"
#include <iostream> 
#include "Grid.h"
#include "Calc.h"
#include <unordered_map>
//#include <concurrent_unordered_map.h>
//#include <map>
//#include <vector>

//static std::vector<RECT> vec;
//long vecSize = 0;

Grid::Grid()
{
	position.x = 0;//�����
	position.y = 0;//�����
	scalePoint = 16; // ��-��������� ������ ������ 16 ��������
	//scale = 16.0;
	//vec.resize(1000000);

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
	double scale;
	if (scalePoint <1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
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

	return res;
}


void Grid::DrawLine(HDC hDC, Point size)
{
	// ������� �����
	if (scalePoint >= 4)//�� ������ ����� ��� ������ < 4 �������  
	{	
	double scale;
	if (scalePoint <1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	HPEN hPen, OldPen; //����������� ����

	long startFade = 16;
	long grayScale = 222;

	if (scale >= 4 && scale <= startFade) //������ ����������� ������� �����
	{
		grayScale = 222 + (startFade - scale) * 32 / (startFade);
	}

	hPen = CreatePen(PS_SOLID, 1, RGB(grayScale, grayScale, grayScale)); //������ ���� ������� �����
	OldPen = (HPEN)SelectObject(hDC, hPen); //������ �������� �������

	long px = position.x % (long)scale; // x ���������� ������ ������������ ����� 
	long py = position.y % (long)scale; // y ���������� ������ ������������ ����� 

	long wx = size.x / scale + 1; // ���������� ����� �� �����������, ������� � ����
	long wy = size.y / scale + 1; // ���������� ����� �� ���������, ������� � ����

	// ������� ������������ �����
		for (long x = 0; x < wx; x++)
		{
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
	DeleteObject(OldPen);
	}
}

void Grid::AddScale(long x, long y) // ��������� ������� ������������ �� ����� x, y
{
	if (scalePoint >= 32) return; // ������������ ������� = 32 �������

	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	float zx = ((float)(x - position.x)) / scale; // ���������� ���� � �����
	float zy = ((float)(y - position.y)) / scale;

	if (scalePoint == -2) scalePoint = 1;
	else if (scalePoint < -32) scalePoint = scalePoint/2; 
	else scalePoint++;

	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	position.x = x - (float)zx * scale; // ����� ���������� ������ �����, ����� ��� �������� ������ �� ��������� �� ����� ���������������
	position.y = y - (float)zy * scale;

}

void Grid::DecScale(long x, long y) //��������� �������
{
	if (scalePoint <= -32768) return; // ����������� ������� = 1/1048576 �������

	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;

	float zx = ((float)(x - position.x)) / scale; // ���������� ���� � �����
	float zy = ((float)(y - position.y)) / scale;

	if (scalePoint == 1) scalePoint = -2;
	else if (scalePoint <= -32) scalePoint = scalePoint * 2;
	else scalePoint--;

	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	position.x = x - (float)zx * scale; // ����� ���������� ������ ����� 
	position.y = y - (float)zy * scale;
}
//void Grid::FillRectangle(HDC hDC, Calc &calc, RECT& rect)
//{
//	//HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //����� �������� �����, ����������� ������ RGB - ������
//	//RECT rect;
//	//HWND hWnd = WindowFromDC(hDC);
//	//GetClientRect(hWnd, &rect);
//
//	RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.
//	std::unordered_map<LONGLONG, Point>::iterator i;
//	//vecSize = -1;
//
//	for (i = calc.LifePointOut.begin(); i != calc.LifePointOut.end(); i++)
//	{
//		//if (i->second.life) 
//		//{
//			r.left = i->second.x * scale + position.x; //X-���������� �������� ������ ���� ��������������.
//			r.top = i->second.y * scale + position.y; // i->second.y;//Y-���������� �������� ������ ���� ��������������.
//			//r.right = (i->second.x + 1) * scale + position.x;//X-���������� ������� ������� ���� ��������������.
//			//r.bottom = (i->second.y + 1) * scale + position.y; //Y-���������� ������� ������� ���� ��������������.
//			if (scale < 1)
//			{
//				r.right = r.left+1;//X-���������� ������� ������� ���� ��������������.
//				r.bottom = r.top+1; //Y-���������� ������� ������� ���� ��������������.
//
//			}
//			else
//			{
//				r.right = (i->second.x + 1) * scale + position.x;//X-���������� ������� ������� ���� ��������������.
//				r.bottom = (i->second.y + 1) * scale + position.y; //Y-���������� ������� ������� ���� ��������������.
//			}
//
//			//��������� �������������
//			if (r.right >= rect.left && r.bottom >= rect.top && r.left <= rect.right && r.top <= rect.bottom)
//			{
//				//vecSize++;
//				//if (vecSize>= vec.size()) vec.resize(1000000);
//				//vec[vecSize]=r;
//				//SetPixel(hDC, r.left, r.top, RGB(0,0,0));
//				//�������� �������� �� �������������� �������� ����� � ��� �� �����!!!
//					InvertRect(hDC, &r); //42
//
//			}
//	//		//DrawFocusRect(hDC, &r); //��� 50% �������������� ������� //23
//	//		//FillRect(hDC, &r, s); //��� 50% �������������� ������� //30
//	//		//Rectangle(hDC, r.left, r.top, r.right, r.bottom); //27
//
//	//		//����� ������
//	//		//MoveToEx(hDC, r.left, r.top, NULL);  // ��� ����� 27
//	//		//LineTo(hDC, r.right, r.bottom);
//
//	//		//SetPixel(hDC, r.left, r.top, RGB(255, 51, 0)); //$$$SetPixel ������������� �������� ���� � ����� � ���������� ������������, GetPixel �������������� ���������� ���� //27
//		//}
//	}
//	//DeleteObject(s);
//
//
//}

void Grid::Draw(unsigned char* &lpBitmapBits, Calc &calc, RECT& rect)
{
	Point size;
	size.x = rect.right - rect.left+1;
	size.y = rect.bottom - rect.top+1;
	int index;
	long indexMax = size.x*size.y * 4;
	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.
	std::unordered_map<LONGLONG, Point>::iterator i;
	for (i = calc.LifePointOut.begin(); i != calc.LifePointOut.end(); i++)
	{
		//r.left = i->second.x * scale + position.x; //X-���������� �������� ������ ���� ��������������.
		//r.top = i->second.y * scale + position.y; //Y-���������� �������� ������ ���� ��������������.
		////r.right = (i->second.x + 1) * scale + position.x;//X-���������� ������� ������� ���� ��������������.
		////r.bottom = (i->second.y + 1) * scale + position.y; //Y-���������� ������� ������� ���� ��������������.

		if (i->second.x < 0 && scale<1) //��� ������������� ����������� � ���������� ����� 1��� - ���������� � ������� ������� (������������� ������� ������ ����������� ������� �����)
			r.left = (double)i->second.x * scale+1 + position.x; //?
		else
			r.left = (double)i->second.x * scale + position.x; //?

		if (i->second.y < 0 && scale<1)
			r.top = (double)i->second.y * scale+1 + position.y; //?
		else
			r.top = (double)i->second.y * scale + position.y; //?


		//if (scale <= 1)
		//{
		//	r.right = r.left;// +1;//X-���������� ������� ������� ���� ��������������.
		//	r.bottom = r.top;// +1; //Y-���������� ������� ������� ���� ��������������.
		//}
		//else
		//{
			r.right = r.left + scale;//X-���������� ������� ������� ���� ��������������.
			r.bottom = r.top + scale; //Y-���������� ������� ������� ���� ��������������.
		//}

		//��������� �������������
		long yMax= scale;
		long xMax= scale;
		if (scale < 1) { yMax = 1; xMax = 1; }

		if (r.right >= rect.left && r.bottom >= rect.top && r.left <= rect.right && r.top <= rect.bottom)
		{
			for (long y = 0; y < yMax; y++)  //������ �������
			{
				for (long x = 0; x < xMax; x++)
				{
					index = size.x * 4 * (r.top+y) + (r.left+x) * 4;
					if (index >= 0 && index <= indexMax)
					{
						lpBitmapBits[index + 0] = 0; // blue
						lpBitmapBits[index + 1] = 0; // green
						lpBitmapBits[index + 2] = 0; // red 
					}
				}
			}
		}
	}
}