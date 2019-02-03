#pragma once
#include <unordered_map>
#include <concurrent_unordered_map.h>
#include <map>
#include "Point.h"
class Calc
{
public:
	
	std::unordered_map <LONGLONG, Point> LifePoint;
	//std::unordered_map <LONGLONG,Point> loudLifePoint;

	Calc();
	~Calc();
	LONGLONG HashPoint(Point point);
	void Insert(Point point, std::unordered_map <LONGLONG, Point> &LifePoint, bool pointDelete);
	void InsertRun(std::unordered_map<LONGLONG, Point>::iterator i, bool pointDelete);
	bool Contains(Point point, std::unordered_map <LONGLONG, Point> &LifePoint);
	void RunLife();
	void DelLife();
	long Generation();
	long population;
	//long& Generation2;
	//bool& Pause();
	long AreaXmin();
	long AreaYmin();
	long AreaXmax();
	long AreaYmax();
	//std::unordered_map& LifePoint();
};

