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
	scale = 15; // По-умолчанию ширина клетки 15 пикселей
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

	//res.x = (hDCPosition.x - position.x) / scale; // Координаты мыши в гриде
	//res.y = (hDCPosition.y - position.y) / scale;

	return res;
}


void Grid::Draw(HDC hDC, Point size)
{
	HPEN hPen, OldPen; //Объявляется перо

	long startFade = 12;
	long grayScale = 222;

	if (scale > 4 && scale < startFade) //делаем постепенный переход сетки
	{
		grayScale = 222 + (startFade - scale) * 32 / (startFade - 5);
	}

	hPen = CreatePen(PS_SOLID, 1, RGB(grayScale, grayScale, grayScale)); //создаём перо нужного цвета
	OldPen = (HPEN)SelectObject(hDC, hPen); //Объект делается текущим
	   	 
	long px = position.x % scale; // x координата первой вертикальной линии 
	long py = position.y % scale; // y координата первой вертикальной линии 

	long wx = size.x / scale + 1; // количество линий по горизонтали, видимых а окне
	long wy = size.y / scale + 1; // количество линий по вертикали, видимых а окне
	// Выводим сетку
	if (scale > 4)//не рисуем сетку при ячейке <= 4 пикселя  
	{	
	// Сначала вертикальные линии
		for (long x = 0; x < wx; x++)
		{
			//SetPixel(hDC, px + x * scale, 0, RGB(255, 51, 0)); //$$$SetPixel устанавливает заданный цвет в точке с указанными координатами, GetPixel соответственно возвращает цвет
			MoveToEx(hDC, px + x * scale, 0, NULL); // Поставим точку
			LineTo(hDC, px + x * scale, size.y); // От неё проведем линию
		}
		// Потом горизонтальные
		for (long y = 0; y < wy; y++)
		{
			MoveToEx(hDC, 0, py + y * scale, NULL);
			LineTo(hDC, size.x, py + y * scale);
		}
	}
	SelectObject(hDC, OldPen);
	DeleteObject(hPen);
	DeleteObject(OldPen);

	//MoveToEx(hDC, 0, 0, NULL);  // Для теста
	//LineTo(hDC, size.x, size.y);

}

void Grid::AddScale(long x, long y) // Увеличить масштаб отталкиваясь от точки x, y
{
	if (scale == 50) return; // максимальный масштаб = 50 пикселей
	
	float zx = ((float)(x - position.x)) / scale; // Координаты мыши в гриде
	float zy = ((float)(y - position.y)) / scale;

	scale++;

	position.x = x - zx * scale; // Новые координаты центра грида, чтобы под курсором ячейка не двигалась во время масштабирования
	position.y = y - zy * scale;

}

void Grid::DecScale(long x, long y)
{
	if (scale == 1) return; // минимальный масштаб = 1 пиксель

	float zx = ((float)(x - position.x)) / scale; // Координаты мыши в гриде
	float zy = ((float)(y - position.y)) / scale;

	scale--;

	position.x = x - zx * scale; // Новые координаты центра грида 
	position.y = y - zy * scale;
}
void Grid::FillRectangle(HDC hDC, Calc &calc, RECT& rect)
{
	//HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //задаём сплошную кисть, закрашенную цветом RGB - черный
	//RECT rect;
	//HWND hWnd = WindowFromDC(hDC);
	//GetClientRect(hWnd, &rect);
	RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
	std::unordered_map<LONGLONG, Point>::iterator i;
	//vecSize = -1;

	for (i = calc.LifePointOut.begin(); i != calc.LifePointOut.end(); i++)
	{
		//if (i->second.life) 
		//{
			r.left = i->second.x * scale + position.x; //X-координата верхнего левого угла прямоугольника.
			r.top = i->second.y * scale + position.y; // i->second.y;//Y-координата верхнего левого угла прямоугольника.
			r.right = (i->second.x + 1) * scale + position.x;//X-координата нижнего правого угла прямоугольника.
			r.bottom = (i->second.y + 1) * scale + position.y; //Y-координата нижнего правого угла прямоугольника.

			//Заполняем прямоугольник
			if (r.right >= rect.left && r.bottom >= rect.top && r.left <= rect.right && r.top <= rect.bottom)
			{
				//vecSize++;
	//		//	//if (vecSize>= vec.size()) vec.resize(1000000);
	//		//	//vec[vecSize]=r;

			InvertRect(hDC, &r); //42
			}
	//		//DrawFocusRect(hDC, &r); //жрёт 50% процессороного времени //23
	//		//FillRect(hDC, &r, s); //жрёт 50% процессороного времени //30
	//		//Rectangle(hDC, r.left, r.top, r.right, r.bottom); //27

	//		//через строку
	//		//MoveToEx(hDC, r.left, r.top, NULL);  // Для теста 27
	//		//LineTo(hDC, r.right, r.bottom);

	//		//SetPixel(hDC, r.left, r.top, RGB(255, 51, 0)); //$$$SetPixel устанавливает заданный цвет в точке с указанными координатами, GetPixel соответственно возвращает цвет //27
		//}
	}
	//DeleteObject(s);


}


//void Grid::FillRectangle2(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBlackBrush, Calc &calc)
//{
//	//HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //задаём сплошную кисть, закрашенную цветом RGB - черный
//	RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
//	std::unordered_map<LONGLONG, Point>::iterator i;
//
//	for (i = calc.LifePoint.begin(); i != calc.LifePoint.end(); i++)
//	{
//		if (i->second.life)
//		{
//			r.left = i->second.x * scale + position.x; //X-координата верхнего левого угла прямоугольника.
//			r.top = i->second.y * scale + position.y; // i->second.y;//Y-координата верхнего левого угла прямоугольника.
//			r.right = (i->second.x + 1) * scale + position.x;//X-координата нижнего правого угла прямоугольника.
//			r.bottom = (i->second.y + 1) * scale + position.y; //Y-координата нижнего правого угла прямоугольника.
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
