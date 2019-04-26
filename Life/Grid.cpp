#include "stdafx.h"
#include <iostream> 
#include "Grid.h"

static Point sizeBuffer;
static long* PointCount ;

Grid::Grid()
{
	position.x = 0;//центр
	position.y = 0;//центр
	scalePoint = 16; // По-умолчанию ширина клетки 16 пикселей
	updateBuffer = true;
	updateInfo = true;
	bool zoom = false;
	bool autoZoom = false;
}

Grid::~Grid()
{
}

union ULL
{
	LONGLONG U;
	long L[2];
};

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

void Grid::AddScale(long x, long y) // Увеличить масштаб отталкиваясь от точки x, y
{
	if (scalePoint >= 32) return; // максимальный масштаб = 32 пикселя

	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	double zx = ((double)(x - position.x)) / scale; // Координаты мыши в гриде
	double zy = ((double)(y - position.y)) / scale;

	if (scalePoint == -2) scalePoint = 1;
	else if (scalePoint < -32) scalePoint = scalePoint/2; 
	else ++scalePoint;

	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	position.x = x - (double)zx * scale; // Новые координаты центра грида, чтобы под курсором ячейка не двигалась во время масштабирования
	position.y = y - (double)zy * scale;

}

void Grid::DecScale(long x, long y) //уменьшить масштаб
{
	if (scalePoint <= -32768) return; // минимальный масштаб = 1/1048576 пиксель

	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;

	double zx = ((double)(x - position.x)) / scale; // Координаты мыши в гриде
	double zy = ((double)(y - position.y)) / scale;

	if (scalePoint == 1) scalePoint = -2;
	else if (scalePoint <= -32) scalePoint = scalePoint * 2;
	else --scalePoint;

	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	position.x = x - (double)zx * scale; // Новые координаты центра грида 
	position.y = y - (double)zy * scale;
}

void Grid::Draw(RECT& rect, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>& LifePoint, long& AreaXmin,long& AreaYmin,long& AreaXmax,long& AreaYmax)
{
		/*Point size;*/
	sizeBuffer.x = rect.right - rect.left + 1;
	sizeBuffer.y = rect.bottom - rect.top + 1;
	int index;
	long indexMax = sizeBuffer.x*sizeBuffer.y * 4;
	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	ULL ull;

	//если ареал по живым
	if (areaLife)//если ареал по живым
	{
		AreaXmin = LONG_MAX;
		AreaYmin = LONG_MAX;
		AreaXmax = LONG_MIN;
		AreaYmax = LONG_MIN;
	}

	if (updateBuffer) //если перезаполняем буфер
	{
		delete[] PointCount;
		PointCount = new long[(sizeBuffer.x+32)*(sizeBuffer.y+32)]();

		// Выводим сетку
		if (scalePoint >= 3)//рисуем сетку при 3х и более пикселей на клетку  
		{
			unsigned char startFade = 16;
			unsigned char grayScale = 222;

			if (scale >= 3 && scale <= startFade) //делаем постепенный переход сетки
			{
				grayScale = 222 + (startFade - scale) * 32 / ((startFade)-2);
			}

			long px = (position.x % (long)scale) - 1 - scale; // x координата первой горизонтальной линии 
			long py = (position.y % (long)scale) - 1 - scale; // y координата первой вертикальной линии 
			long yMax = sizeBuffer.y;
			long xMax = sizeBuffer.x;

			//горизонтальные линии
			for (long y = py; y < yMax; y += scalePoint)
			{
				for (long x = px; x < xMax; x = x++)
				{
					index = xMax * 4 * y + x * 4;
					if (x >= 0 && y >= 0)
					{
						lpBitmapBits[index + 0] = grayScale; // blue
						lpBitmapBits[index + 1] = grayScale; // green
						lpBitmapBits[index + 2] = grayScale; // red 
					}
				}
			}

			//вертикальные линии
			for (long x = px; x < xMax; x += scalePoint)
			{
				for (long y = py; y < yMax; ++y)
				{
					index = xMax * 4 * y + x * 4;
					if (x >= 0 && y >= 0)
					{
						lpBitmapBits[index + 0] = grayScale; // blue
						lpBitmapBits[index + 1] = grayScale; // green
						lpBitmapBits[index + 2] = grayScale; // red 
					}
				}
			}
		}

		//выводим живые клетки
		std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
		for (i = LifePoint.begin(); i != LifePoint.end(); ++i)
		{
			if (((i->second[SIZE_POINT-1]  >> 6) & 1) == 1)//(i->second.life)
			{
				RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
				ull.U = i->first;

				if (ull.L[0] < 0 && scale < 1) //при отрицательных координатах и масштабах менее 1пик - округление в меньшую сторону (целочисленное деление просто отбрасывает дробную часть)
					r.left = (double)ull.L[0] * scale + 1 + position.x; //X-координата верхнего левого угла прямоугольника.
				else
					r.left = (double)ull.L[0] * scale + position.x; //?

				if (ull.L[1] < 0 && scale < 1)
					r.top = (double)ull.L[1] * scale + 1 + position.y; //Y-координата верхнего левого угла прямоугольника.
				else
					r.top = (double)ull.L[1] * scale + position.y; //?

				r.right = r.left + scale;//X-координата нижнего правого угла прямоугольника.
				r.bottom = r.top + scale; //Y-координата нижнего правого угла прямоугольника.
				long yMax = scale - 1;
				long xMax = scale - 1;
				if (scale <= 1)
				{
					yMax = 1;
					xMax = 1;
				}
				else if (scale == 2)
				{
					yMax = 2;
					xMax = 2;
				}

				if (r.right >= 0 && r.bottom >= 0 && r.left <= rect.right && r.top <= rect.bottom)
				{
					for (long y = 0; y < yMax; ++y)  //рисуем квадрат
					{
						for (long x = 0; x < xMax; ++x)
						{
							long xx = r.left + x;
							long yy = r.top + y;
							index = sizeBuffer.x * 4 * yy + xx * 4;
							if (xx >= 0 && yy >= 0 && xx < sizeBuffer.x && yy < sizeBuffer.y)
							{
								lpBitmapBits[index + 0] = 0; // blue
								lpBitmapBits[index + 1] = 0; // green
								lpBitmapBits[index + 2] = 0; // red 
							}
						}
					}
					//добавляем данные в PointCount о количестве ячеек в пикселе
					PointCount[(r.top + 32)*(sizeBuffer.x + 32) + (r.left + 32)]++;
				}

				//если ареал по живым
				if (areaLife)//если ареал по живым
				{
					if (AreaXmin > ull.L[0])AreaXmin = ull.L[0];//расчет ареала 
					if (AreaYmin > ull.L[1])AreaYmin = ull.L[1];
					if (AreaXmax < ull.L[0])AreaXmax = ull.L[0];
					if (AreaYmax < ull.L[1])AreaYmax = ull.L[1];
				}
			}
		}
	} 
}

void Grid::DrawPoint(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i)
{
	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	long indexP;

	//RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
	long AreaXmin;//r.left
	long AreaYmin;//r.top 
	long AreaXmax;//r.right
	long AreaYmax;//r.bottom
	ULL ull;
	ull.U = i->first;

	if (ull.L[0] < 0 && scale < 1) //при отрицательных координатах и масштабах менее 1пик - округление в меньшую сторону (целочисленное деление просто отбрасывает дробную часть)
		AreaXmin = (double)ull.L[0] * scale + 1 + position.x; //X-координата верхнего левого угла прямоугольника.
	else
		AreaXmin = (double)ull.L[0] * scale + position.x; //?

	if (ull.L[1] < 0 && scale < 1)
		AreaYmin = (double)ull.L[1] * scale + 1 + position.y; //Y-координата верхнего левого угла прямоугольника.
	else
		AreaYmin = (double)ull.L[1] * scale + position.y; //?

	AreaXmax = AreaXmin + scale;//X-координата нижнего правого угла прямоугольника.
	AreaYmax = AreaYmin + scale; //Y-координата нижнего правого угла прямоугольника.

	if (AreaXmax >= 0 && AreaYmax >= 0 && AreaXmin < sizeBuffer.x && AreaYmin < sizeBuffer.y)
	{
		indexP = (AreaYmin + 32)*(sizeBuffer.x + 32) + (AreaXmin + 32);
		if (((i->second[SIZE_POINT-1]  >> 6) & 1) == 1)	++PointCount[indexP];//добавляем данные в PointCount о количестве ячеек в пикселе
		else --PointCount[indexP];//добавляем данные в PointCount о количестве ячеек в пикселе
		
		if (PointCount[indexP] == 1) //отрисовываем квадрат
		{
			long yMax = scale - 1;
			long xMax = scale - 1;
			if (scale <= 1)
			{
				yMax = 1;
				xMax = 1;
			}
			else if (scale == 2)
			{
				yMax = 2;
				xMax = 2;
			}

			for (long y = 0; y < yMax; ++y)  //рисуем квадрат
			{
				for (long x = 0; x < xMax; ++x)
				{
					long xx = AreaXmin + x;
					long yy = AreaYmin + y;
					long index = sizeBuffer.x * 4 * yy + xx * 4;
					if (xx >= 0 && yy >= 0 && xx < sizeBuffer.x && yy < sizeBuffer.y)
					{
						lpBitmapBits[index + 0] = 0; // blue
						lpBitmapBits[index + 1] = 0; // green
						lpBitmapBits[index + 2] = 0; // red 
					}
				}
			}
		}
		else if (PointCount[indexP] == 0) //стираем квадрат
		{
			long yMax = scale - 1;
			long xMax = scale - 1;
			if (scale <= 1)
			{
				yMax = 1;
				xMax = 1;
			}
			else if (scale == 2)
			{
				yMax = 2;
				xMax = 2;
			}

			for (long y = 0; y < yMax; ++y)  //рисуем белого цвета
			{
				for (long x = 0; x < xMax; ++x)
				{
					long xx = AreaXmin + x;
					long yy = AreaYmin + y;
					long index = sizeBuffer.x * 4 * yy + xx * 4;
					if (xx >= 0 && yy >= 0 && xx < sizeBuffer.x && yy < sizeBuffer.y)
					{
						lpBitmapBits[index + 0] = 255; // blue
						lpBitmapBits[index + 1] = 255; // green
						lpBitmapBits[index + 2] = 255; // red 
					}
				}
			}
		}
	}
}