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
	position.x = 0;//центр
	position.y = 0;//центр
	scalePoint = 16; // По-умолчанию ширина клетки 16 пикселей
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
	if (xTmp < 0) //при отрицательных координатах округление в меньшую сторону (целочисленное деление просто отбрасывает дробную часть)
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
	// Выводим сетку
	if (scalePoint >= 4)//не рисуем сетку при ячейке < 4 пикселя  
	{	
	double scale;
	if (scalePoint <1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	HPEN hPen, OldPen; //Объявляется перо

	long startFade = 16;
	long grayScale = 222;

	if (scale >= 4 && scale <= startFade) //делаем постепенный переход сетки
	{
		grayScale = 222 + (startFade - scale) * 32 / (startFade);
	}

	hPen = CreatePen(PS_SOLID, 1, RGB(grayScale, grayScale, grayScale)); //создаём перо нужного цвета
	OldPen = (HPEN)SelectObject(hDC, hPen); //Объект делается текущим

	long px = position.x % (long)scale; // x координата первой вертикальной линии 
	long py = position.y % (long)scale; // y координата первой вертикальной линии 

	long wx = size.x / scale + 1; // количество линий по горизонтали, видимых а окне
	long wy = size.y / scale + 1; // количество линий по вертикали, видимых а окне

	// Сначала вертикальные линии
		for (long x = 0; x < wx; x++)
		{
			MoveToEx(hDC, px + x * scale, 0, NULL); // Поставим точку
			LineTo(hDC, px + x * scale, size.y); // От неё проведем линию
		}
		// Потом горизонтальные
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

void Grid::AddScale(long x, long y) // Увеличить масштаб отталкиваясь от точки x, y
{
	if (scalePoint >= 32) return; // максимальный масштаб = 32 пикселя

	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	float zx = ((float)(x - position.x)) / scale; // Координаты мыши в гриде
	float zy = ((float)(y - position.y)) / scale;

	if (scalePoint == -2) scalePoint = 1;
	else if (scalePoint < -32) scalePoint = scalePoint/2; 
	else scalePoint++;

	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	position.x = x - (float)zx * scale; // Новые координаты центра грида, чтобы под курсором ячейка не двигалась во время масштабирования
	position.y = y - (float)zy * scale;

}

void Grid::DecScale(long x, long y) //уменьшить масштаб
{
	if (scalePoint <= -32768) return; // минимальный масштаб = 1/1048576 пиксель

	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;

	float zx = ((float)(x - position.x)) / scale; // Координаты мыши в гриде
	float zy = ((float)(y - position.y)) / scale;

	if (scalePoint == 1) scalePoint = -2;
	else if (scalePoint <= -32) scalePoint = scalePoint * 2;
	else scalePoint--;

	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	position.x = x - (float)zx * scale; // Новые координаты центра грида 
	position.y = y - (float)zy * scale;
}
//void Grid::FillRectangle(HDC hDC, Calc &calc, RECT& rect)
//{
//	//HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //задаём сплошную кисть, закрашенную цветом RGB - черный
//	//RECT rect;
//	//HWND hWnd = WindowFromDC(hDC);
//	//GetClientRect(hWnd, &rect);
//
//	RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
//	std::unordered_map<LONGLONG, Point>::iterator i;
//	//vecSize = -1;
//
//	for (i = calc.LifePointOut.begin(); i != calc.LifePointOut.end(); i++)
//	{
//		//if (i->second.life) 
//		//{
//			r.left = i->second.x * scale + position.x; //X-координата верхнего левого угла прямоугольника.
//			r.top = i->second.y * scale + position.y; // i->second.y;//Y-координата верхнего левого угла прямоугольника.
//			//r.right = (i->second.x + 1) * scale + position.x;//X-координата нижнего правого угла прямоугольника.
//			//r.bottom = (i->second.y + 1) * scale + position.y; //Y-координата нижнего правого угла прямоугольника.
//			if (scale < 1)
//			{
//				r.right = r.left+1;//X-координата нижнего правого угла прямоугольника.
//				r.bottom = r.top+1; //Y-координата нижнего правого угла прямоугольника.
//
//			}
//			else
//			{
//				r.right = (i->second.x + 1) * scale + position.x;//X-координата нижнего правого угла прямоугольника.
//				r.bottom = (i->second.y + 1) * scale + position.y; //Y-координата нижнего правого угла прямоугольника.
//			}
//
//			//Заполняем прямоугольник
//			if (r.right >= rect.left && r.bottom >= rect.top && r.left <= rect.right && r.top <= rect.bottom)
//			{
//				//vecSize++;
//				//if (vecSize>= vec.size()) vec.resize(1000000);
//				//vec[vecSize]=r;
//				//SetPixel(hDC, r.left, r.top, RGB(0,0,0));
//				//добавить проверку на инвертирование повторно одной и той же точки!!!
//					InvertRect(hDC, &r); //42
//
//			}
//	//		//DrawFocusRect(hDC, &r); //жрёт 50% процессороного времени //23
//	//		//FillRect(hDC, &r, s); //жрёт 50% процессороного времени //30
//	//		//Rectangle(hDC, r.left, r.top, r.right, r.bottom); //27
//
//	//		//через строку
//	//		//MoveToEx(hDC, r.left, r.top, NULL);  // Для теста 27
//	//		//LineTo(hDC, r.right, r.bottom);
//
//	//		//SetPixel(hDC, r.left, r.top, RGB(255, 51, 0)); //$$$SetPixel устанавливает заданный цвет в точке с указанными координатами, GetPixel соответственно возвращает цвет //27
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
	RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
	std::unordered_map<LONGLONG, Point>::iterator i;
	for (i = calc.LifePointOut.begin(); i != calc.LifePointOut.end(); i++)
	{
		//r.left = i->second.x * scale + position.x; //X-координата верхнего левого угла прямоугольника.
		//r.top = i->second.y * scale + position.y; //Y-координата верхнего левого угла прямоугольника.
		////r.right = (i->second.x + 1) * scale + position.x;//X-координата нижнего правого угла прямоугольника.
		////r.bottom = (i->second.y + 1) * scale + position.y; //Y-координата нижнего правого угла прямоугольника.

		if (i->second.x < 0 && scale<1) //при отрицательных координатах и масшщтабах менее 1пик - округление в меньшую сторону (целочисленное деление просто отбрасывает дробную часть)
			r.left = (double)i->second.x * scale+1 + position.x; //?
		else
			r.left = (double)i->second.x * scale + position.x; //?

		if (i->second.y < 0 && scale<1)
			r.top = (double)i->second.y * scale+1 + position.y; //?
		else
			r.top = (double)i->second.y * scale + position.y; //?


		//if (scale <= 1)
		//{
		//	r.right = r.left;// +1;//X-координата нижнего правого угла прямоугольника.
		//	r.bottom = r.top;// +1; //Y-координата нижнего правого угла прямоугольника.
		//}
		//else
		//{
			r.right = r.left + scale;//X-координата нижнего правого угла прямоугольника.
			r.bottom = r.top + scale; //Y-координата нижнего правого угла прямоугольника.
		//}

		//Заполняем прямоугольник
		long yMax= scale;
		long xMax= scale;
		if (scale < 1) { yMax = 1; xMax = 1; }

		if (r.right >= rect.left && r.bottom >= rect.top && r.left <= rect.right && r.top <= rect.bottom)
		{
			for (long y = 0; y < yMax; y++)  //рисуем квадрат
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