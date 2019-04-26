#pragma once
#include <unordered_map>
#include "Grid.h"
#include "Point.h"
#include "Msg.h"

class Calc
{
public:
	Calc();
	~Calc();

	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> LifePoint;
	LONGLONG HashPoint(Point point);
	int Insert(Point point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint, bool pointDelete, Grid& grid);
	int InsertRun(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i, bool pointDelete, Grid& grid);
	bool Contains(Point point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint);
	int RunLife(Grid& grid);
	void RunLifeStep(ULONGLONG step, Grid& grid);
	void DelLife();
	ULONGLONG Generation;
	long Population;
	long AreaXmin;
	long AreaYmin;
	long AreaXmax;
	long AreaYmax;
	long LifePointSize;
};

