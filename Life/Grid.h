#pragma once
#include "Point.h"
#include <unordered_map>

class Grid
{
public:
	Grid();
	~Grid();

	Point position; // ������� ������ ��������� ����� ������������ ������ �������� ���� ���� � ���������
	long scalePoint; // ������� �������� �������� ���� ������
	void Move(long x, long y); // �������� ���� �� x �������� ������ � y �����
	void AddScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void DecScale(long x, long y); // ��������� ������� ������������ �� ����� x, y
	void Draw(RECT& rect, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>& LifePoint, long& AreaXmin, long& AreaYmin, long& AreaXmax, long& AreaYmax);
	Point GetCell(Point point); // �������� ���������� ����� 
	bool updateBuffer;
	bool updateInfo;
	bool zoom;
	bool autoZoom;
	bool areaLife;
	void DrawPoint(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i);
	unsigned char* lpBitmapBits;
};


