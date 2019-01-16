#pragma once
#include <unordered_map>
class Calc
{
public:
	
	std::unordered_map <LONGLONG, POINT> LifePoint;
	//std::unordered_map <LONGLONG,POINT> loudLifePoint;

	Calc();
	~Calc();

	void Insert(POINT point, bool inver, std::unordered_map <LONGLONG, POINT> &LifePoint);
	bool Contains(POINT point, std::unordered_map <LONGLONG, POINT> &LifePoint);
	void RunLife(bool &RunCalc, bool &CalcEnd);
	void DelLife();
	long& Generation();
	bool& Pause();
};

