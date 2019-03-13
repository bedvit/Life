#include "stdafx.h"
#include <iostream> 
#include "Grid.h"

static Point sizeBuffer;
static long* PointCount ;

Grid::Grid()
{
	position.x = 0;//�����
	position.y = 0;//�����
	scalePoint = 16; // ��-��������� ������ ������ 16 ��������
	updateBuffer = true;
	updateInfo = true;
	bool zoom = false;
	bool autoZoom = false;
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

void Grid::Draw(RECT& rect, std::unordered_map<LONGLONG, Point>& LifePoint, long& AreaXmin,long& AreaYmin,long& AreaXmax,long& AreaYmax)
{
		/*Point size;*/
	sizeBuffer.x = rect.right - rect.left + 1;
	sizeBuffer.y = rect.bottom - rect.top + 1;
	int index;
	long indexMax = sizeBuffer.x*sizeBuffer.y * 4;
	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;

	//���� ����� �� �����
	if (areaLife)//���� ����� �� �����
	{
		AreaXmin = LONG_MAX;
		AreaYmin = LONG_MAX;
		AreaXmax = LONG_MIN;
		AreaYmax = LONG_MIN;
	}

	if (updateBuffer) //���� ������������� �����
	{
		delete[] PointCount;
		PointCount = new long[(sizeBuffer.x+32)*(sizeBuffer.y+32)]();

		// ������� �����
		if (scalePoint >= 3)//������ ����� ��� 3� � ����� �������� �� ������  
		{
			long startFade = 16;
			long grayScale = 222;

			if (scale >= 3 && scale <= startFade) //������ ����������� ������� �����
			{
				grayScale = 222 + (startFade - scale) * 32 / ((startFade)-2);
			}

			long px = (position.x % (long)scale) - 1 - scale; // x ���������� ������ �������������� ����� 
			long py = (position.y % (long)scale) - 1 - scale; // y ���������� ������ ������������ ����� 
			long yMax = sizeBuffer.y;
			long xMax = sizeBuffer.x;

			//�������������� �����
			for (long y = py; y < yMax; y = y + scalePoint)
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

			//������������ �����
			for (long x = px; x < xMax; x = x + scalePoint)
			{
				for (long y = py; y < yMax; y++)
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

		//������� ����� ������
		std::unordered_map<LONGLONG, Point>::iterator i;
		for (i = LifePoint.begin(); i != LifePoint.end(); i++)
		{
			if (i->second.life)
			{
				RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.

				if (i->second.x < 0 && scale < 1) //��� ������������� ����������� � ��������� ����� 1��� - ���������� � ������� ������� (������������� ������� ������ ����������� ������� �����)
					r.left = (double)i->second.x * scale + 1 + position.x; //X-���������� �������� ������ ���� ��������������.
				else
					r.left = (double)i->second.x * scale + position.x; //?

				if (i->second.y < 0 && scale < 1)
					r.top = (double)i->second.y * scale + 1 + position.y; //Y-���������� �������� ������ ���� ��������������.
				else
					r.top = (double)i->second.y * scale + position.y; //?

				r.right = r.left + scale;//X-���������� ������� ������� ���� ��������������.
				r.bottom = r.top + scale; //Y-���������� ������� ������� ���� ��������������.
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
					for (long y = 0; y < yMax; y++)  //������ �������
					{
						for (long x = 0; x < xMax; x++)
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
					//��������� ������ � PointCount � ���������� ����� � �������
					PointCount[(r.top + 32)*(sizeBuffer.x + 32) + (r.left + 32)]++;
				}

				//���� ����� �� �����
				if (areaLife)//���� ����� �� �����
				{
					if (AreaXmin > i->second.x)AreaXmin = i->second.x;//������ ������ 
					if (AreaYmin > i->second.y)AreaYmin = i->second.y;
					if (AreaXmax < i->second.x)AreaXmax = i->second.x;
					if (AreaYmax < i->second.y)AreaYmax = i->second.y;
				}
			}
		}
	} 
}

void Grid::DrawPoint(Point &point)
{
	double scale;
	if (scalePoint < 1) scale = (double)-1.00 / scalePoint; else scale = scalePoint;
	long indexP;

	RECT r; //��������� ��������� ��������� RECT - ���������� ��������������.

	if (point.x < 0 && scale < 1) //��� ������������� ����������� � ��������� ����� 1��� - ���������� � ������� ������� (������������� ������� ������ ����������� ������� �����)
		r.left = (double)point.x * scale + 1 + position.x; //X-���������� �������� ������ ���� ��������������.
	else
		r.left = (double)point.x * scale + position.x; //?

	if (point.y < 0 && scale < 1)
		r.top = (double)point.y * scale + 1 + position.y; //Y-���������� �������� ������ ���� ��������������.
	else
		r.top = (double)point.y * scale + position.y; //?

	r.right = r.left + scale;//X-���������� ������� ������� ���� ��������������.
	r.bottom = r.top + scale; //Y-���������� ������� ������� ���� ��������������.

	if (r.right >= 0 && r.bottom >= 0 && r.left < sizeBuffer.x && r.top < sizeBuffer.y)
	{
		indexP = (r.top + 32)*(sizeBuffer.x + 32) + (r.left + 32);
		if (point.life)	++PointCount[indexP];//��������� ������ � PointCount � ���������� ����� � �������
		else --PointCount[indexP];//��������� ������ � PointCount � ���������� ����� � �������
		
		if (PointCount[indexP] == 1) //������������ �������
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

			for (long y = 0; y < yMax; y++)  //������ �������
			{
				for (long x = 0; x < xMax; x++)
				{
					long xx = r.left + x;
					long yy = r.top + y;
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
		else if (PointCount[indexP] == 0) //������� �������
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

			for (long y = 0; y < yMax; y++)  //������ ������ �����
			{
				for (long x = 0; x < xMax; x++)
				{
					long xx = r.left + x;
					long yy = r.top + y;
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