#pragma once
#include <unordered_map>
#include "Grid.h"
#include "Point.h"

class Calc
{
public:
	Calc();
	~Calc();

	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> LifePoint;
	LONGLONG HashPoint(Point point);
	void Insert(Point point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint, bool pointDelete, Grid& grid);
	void InsertRun(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i, bool pointDelete, Grid& grid);
	bool Contains(Point point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint);
	void RunLife(Grid& grid);
	void RunLifeStep(long step, Grid& grid);
	void DelLife();
	long Generation;
	long Population;
	long AreaXmin;
	long AreaYmin;
	long AreaXmax;
	long AreaYmax;
};

