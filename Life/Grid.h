#pragma once
//#include "Point.h"
//#include "Calc.h"
#include "Point.h"
#include <unordered_map>

class Grid
{
public:
	Grid();
	~Grid();

	Point position; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях
	long scalePoint; // сколько пикселей занимает одна клетка
	void DrawLine(HDC hdc, Point size); // Нарисовать сетку
	void Move(long x, long y); // Сдвигает грид на x пикселей вправо и y вверх
	void AddScale(long x, long y); // Увеличить масштаб отталкиваясь от точки x, y
	void DecScale(long x, long y); // Уменьшить масштаб отталкиваясь от точки x, y
	void Draw(unsigned char*& lpBitmapBits, std::unordered_map <LONGLONG, Point>& LifePoint, RECT& rect);
	Point GetCell(Point); // Получить координаты точки 
	bool updateBuffer;
	void DrawPoint(Point &point);
};


