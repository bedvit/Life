#pragma once
#include <unordered_map>
#include <concurrent_unordered_map.h>
#include <map>
class Calc
{
public:
	
	std::unordered_map <LONGLONG, POINT> LifePoint;
	//std::unordered_map <LONGLONG,POINT> loudLifePoint;

	Calc();
	~Calc();
	LONGLONG HashPoint(POINT point);
	void Insert(POINT point, std::unordered_map <LONGLONG, POINT> &LifePoint, bool inver);
	bool Contains(POINT point, std::unordered_map <LONGLONG, POINT> &LifePoint);
	void RunLife();
	void DelLife();
	long Generation();
	//long& Generation2;
	//bool& Pause();
	long AreaXmin();
	long AreaYmin();
	long AreaXmax();
	long AreaYmax();
	//std::unordered_map& LifePoint();
};

