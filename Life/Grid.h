#pragma once
//#include "Point.h"
#include "Calc.h"
#include "Point.h"

class Grid
{
public:

	Point position; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях
	long scale; // сколько пикселей занимает одна клетка


	Grid();
	~Grid();

	void Draw(HDC hdc, Point size); // Нарисовать сетку
	void Move(long x, long y); // Сдвигает грид на x пикселей вправо и y вверх
	void AddScale(long x, long y); // Увеличить масштаб отталкиваясь от точки x, y
	void DecScale(long x, long y); // Уменьшить масштаб отталкиваясь от точки x, y
	void FillRectangle(HDC hDC, Calc& calc); //Закрашмваем многоугольник
	
	Point GetCell(Point); // Получить координаты точки 
	void DrawRect(HDC, HBRUSH, Point);



};


