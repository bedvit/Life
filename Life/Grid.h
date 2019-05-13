#pragma once
#include "Point.h"
#include <unordered_map>

class Grid
{
public:
	Grid();
	~Grid();

	Point position; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях - LONGLONG
	long scalePoint; // сколько пикселей занимает одна клетка
	void Move(LONGLONG x, LONGLONG y); // Сдвигает грид на x пикселей вправо и y вверх
	void AddScale(LONGLONG x, LONGLONG y); // Увеличить масштаб отталкиваясь от точки x, y
	void DecScale(LONGLONG x, LONGLONG y); // Уменьшить масштаб отталкиваясь от точки x, y
	void Draw(RECT& rect, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>& LifePoint, long& AreaXmin, long& AreaYmin, long& AreaXmax, long& AreaYmax);
	POINT GetCell(Point point); // Получить координаты точки 
	bool OutRange(Point point);
	bool updateBuffer;
	bool updateInfo;
	bool zoom;
	bool autoZoom;
	bool areaLife;
	void DrawPoint(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i);
	unsigned char* lpBitmapBits;
	//void PositionLoop();
};


