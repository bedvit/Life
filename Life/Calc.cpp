#include "stdafx.h"
#include "Calc.h"
#include <unordered_map>
#include <thread>
#include "windows.h"
#include "dos.h"
#include <deque>


//CRITICAL_SECTION cs;//критическая секция


static long generation=0;
static long areaXmin= LONG_MAX;
static long areaYmin= LONG_MAX;
static long areaXmax= LONG_MIN;
static long areaYmax= LONG_MIN;
//static std::deque<POINT> loudLifePoint;//облако
//static std::vector<POINT> loudLifePoint;//облако

static std::unordered_map <LONGLONG, POINT> LifePointRun; //предыдущее поколение


Calc::Calc()
{
	//Generation2= generation;
	//LifePoint;

}

LONGLONG Calc::HashPoint(POINT point)
{
	return (LONGLONG)point.x * 1000000000 + point.y;
}

void Calc::Insert(POINT point, std::unordered_map <LONGLONG, POINT> &LifePoint, bool invert)

{
	LONGLONG tmp = HashPoint(point);
	if (invert)
	{
		LifePoint.erase(tmp);
	}
	else
	{
		LifePoint[tmp] = point;
		if (areaXmin > point.x)areaXmin = point.x;//расчет ареала 
		if (areaYmin > point.y)areaYmin = point.y;
		if (areaXmax < point.x)areaXmax = point.x;
		if (areaYmax < point.y)areaYmax = point.y;
	}
}

long Calc::Generation(){ return generation;}
long Calc::AreaXmin() { return areaXmin; }
long Calc::AreaYmin() { return areaYmin; }
long Calc::AreaXmax() { return areaXmax; }
long Calc::AreaYmax() { return areaYmax; }

//bool& Calc::Pause() { return pause; }

bool Calc::Contains(POINT point, std::unordered_map <LONGLONG, POINT> &LifePoint) //аналог std::unordered_map::contains (C++20)

{
	LONGLONG tmp = HashPoint(point);
	if (LifePoint.count(tmp) == 1) return true; else return false;

}


void Run(std::unordered_map <LONGLONG, POINT> &LifePoint)
{

	//while (RunCalc && !CalcEnd) {
	//	if (!Pause)
	//	{
			int n[3] = { -1, 0, 1 };
			POINT tmpPoint;// = { 0,0 };
			Calc calc;
			int L=0;
			//long iter;
			std::unordered_map <LONGLONG, POINT> LifePointLoud; //облако
			//EnterCriticalSection(&cs);//критическая секция
			std::unordered_map <LONGLONG, POINT> LifePointOld(LifePoint); //предыдущее поколение
			//LeaveCriticalSection(&cs);//критическая секция
			std::unordered_map<LONGLONG, POINT>::iterator i;

			if (generation==0)
			{
				LifePointRun = LifePointOld;
			}
		

			for (i = LifePointRun.begin(); i != LifePointRun.end(); i++) //заполняем облако
			{
				for (int y = 0; y < 3; y++) //обработка 9 координат
				{
					for (int x = 0; x < 3; x++)
					{
						tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
						calc.Insert(tmpPoint, LifePointLoud, false);
						//loudLifePoint[iter] = tmpPoint;
						//iter++;
					}
				}
			}
			LifePointRun.clear();
			for (i = LifePointLoud.begin(); i != LifePointLoud.end(); i++) //считаем новое поколение по облаку
			{
				for (int y = 0; y < 3; y++) //обработка 9 координат
				{
					for (int x = 0; x < 3; x++)
					{
						if (!(y == 1 && x == 1))
						{
							
							
								tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
								if (calc.Contains(tmpPoint, LifePointOld))	L++;
								if (L > 3) goto nx_; //нет смысла считать более 4 точек вокруг
						}
					}
				}
				//EnterCriticalSection(&cs);//критическая секция
				nx_:
				if (L < 2)
				{
					if (calc.Contains(i->second, LifePointOld)) //если точка была
					{
						calc.Insert(i->second, LifePoint, true);
						calc.Insert(i->second, LifePointRun, false);
					}
				}
				else if (L == 2)
				{
					
				}
				else if (L == 3)
				{
					if (!calc.Contains(i->second, LifePointOld)) //если точки не было
					{
						calc.Insert(i->second, LifePoint, false);
						calc.Insert(i->second, LifePointRun, false);
					}
				}
				else if (L > 3)
				{
					if (calc.Contains(i->second, LifePointOld)) //если точка была
					{
						calc.Insert(i->second, LifePoint, true);
						calc.Insert(i->second, LifePointRun, false);
					}
				}
				//LeaveCriticalSection(&cs);//критическая секция
				L = 0;
			}
			//EnterCriticalSection(&cs);//критическая секция
			++generation;
			//CalcEnd = true;
			//LeaveCriticalSection(&cs);//критическая секция
		//}
	//}
}

void Calc::RunLife()

{
	//if (!Pause)
	//{
		////InitializeCriticalSection(&cs);
		//std::vector<std::thread> thr(2); //для запуска двух потоков
		//for (int i = 1; i <= 1; i++) thr[i - 1] = std::thread(Run, std::ref(RunCalc), std::ref(CalcEnd), std::ref(Pause), std::ref(this->LifePoint));
		//for (int i = 1; i <= 1; i++) thr[i - 1].detach(); //отделяем потоки
		////for (int i = 1; i <= 1; i++) thr[i - 1].join(); //ждем все потоки


		Run(std::ref(this->LifePoint));
	//}
}

void Calc::DelLife()

{
	this->LifePoint.clear();
	generation = 0;
	areaXmin = LONG_MAX;
	areaYmin = LONG_MAX;
	areaXmax = LONG_MIN;
	areaYmax = LONG_MIN;
}

Calc::~Calc()
{
}


