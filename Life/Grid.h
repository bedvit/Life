#pragma once
#include "Point.h"
#include <unordered_map>

class Grid
{
public:
	Grid();
	~Grid();

	Point position; // ������� ������ ��������� ����� ������������ ������ �������� ���� ���� � ��������� - LONGLONG
	long scalePoint; // ������� �������� �������� ���� ������
	void Move(LONGLONG x, LONGLONG y); // �������� ���� �� x �������� ������ � y �����
	void AddScale(LONGLONG x, LONGLONG y); // ��������� ������� ������������ �� ����� x, y
	void DecScale(LONGLONG x, LONGLONG y); // ��������� ������� ������������ �� ����� x, y
	void Draw(RECT& rect, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>& LifePoint, long& AreaXmin, long& AreaYmin, long& AreaXmax, long& AreaYmax);
	POINT GetCell(Point point); // �������� ���������� ����� 
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


