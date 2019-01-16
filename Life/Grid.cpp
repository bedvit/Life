#include "stdafx.h"
#include <iostream> 
#include "Grid.h"
#include "Calc.h"
#include <unordered_map>

Grid::Grid()
{
	position.x = 0;
	position.y = 0;
	scale = 15; // По-умолчанию ширина клетки 15 пикселей

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


void Grid::Draw(HDC hDC, POINT size)
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
	if (scale <= 4) return; //не рисуем сетку при ячейке <= 4 пикселя  
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

	SelectObject(hDC, OldPen);
	DeleteObject(hPen);

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
void Grid::FillRectangle(HDC hDC, Calc calc)
{
	HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //задаём сплошную кисть, закрашенную цветом RGB - черный

	std::unordered_map<LONGLONG, POINT>::iterator i;

	for (i = calc.LifePoint.begin(); i != calc.LifePoint.end(); i++)
	{
		DrawRect(hDC, s, i->second);
	}

	DeleteObject(s);
}

void Grid::DrawRect(HDC hDC, HBRUSH s, POINT pos)
{

	RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
	r.left = pos.x * scale + position.x; //X-координата верхнего левого угла прямоугольника.
	r.top = pos.y * scale + position.y; // i->second.y;//Y-координата верхнего левого угла прямоугольника.
	r.right = (pos.x + 1) * scale + position.x;//X-координата нижнего правого угла прямоугольника.
	r.bottom = (pos.y + 1) * scale + position.y; //Y-координата нижнего правого угла прямоугольника.

	//Заполняем прямоугольник
	FillRect(hDC, &r, s);

}

