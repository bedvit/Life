#pragma once
#include <unordered_map>
//#include <concurrent_unordered_map.h>
//#include <map>
#include "Point.h"
class Calc
{
public:
	
	std::unordered_map <LONGLONG, Point> LifePoint;
	std::unordered_map <LONGLONG,Point> LifePointOut;

	Calc();
	~Calc();
	LONGLONG HashPoint(Point point);
	void Insert(Point point, std::unordered_map <LONGLONG, Point> &LifePoint, bool pointDelete);
	void InsertRun(std::unordered_map<LONGLONG, Point>::iterator i, bool pointDelete);
	bool Contains(Point point, std::unordered_map <LONGLONG, Point> &LifePoint);
	void RunLife();
	//void RunLifeTread();
	void RunLifeStep(long& step);
	void DelLife();
	long Generation;
	long Population;
	long AreaXmin;;//LONGLONG AreaXmin;
	long AreaYmin;
	long AreaXmax;
	long AreaYmax;
	bool CalcEnd; //вычисления закончены - готов расчет нового поколения
	//void Update();

};

