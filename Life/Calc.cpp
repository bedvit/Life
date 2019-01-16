#include "stdafx.h"
#include "Calc.h"
#include <unordered_map>
#include <thread>
#include "windows.h"
#include "dos.h"


//CRITICAL_SECTION cs;//критическая секция
//std::unordered_map <LONGLONG, POINT> СloudLifePoint;

Calc::Calc()
{
	
	//LifePoint;

}

void Calc::Insert(POINT point, bool invert, std::unordered_map <LONGLONG, POINT> &LifePoint)

{
	LONGLONG tmp = (LONGLONG)point.x * 1000000000 + point.y;

	if (invert)
		LifePoint.erase(tmp);
	else
		LifePoint[tmp] = point;

}

bool Calc::Contains(POINT point, std::unordered_map <LONGLONG, POINT> &LifePoint) //аналог std::unordered_map::contains (C++20)

{
	LONGLONG tmp = (LONGLONG)point.x * 1000000000 + point.y;
	if (LifePoint.count(tmp) == 1) return true; else return false;

}


void Run(bool &CalcEnd, std::unordered_map <LONGLONG, POINT> &LifePoint)
{
	int n[3] = { -1, 0, 1 };
	POINT tmpPoint;// = { 0,0 };
	Calc calc;
	int L=0;
	std::unordered_map <LONGLONG, POINT> loudLifePoint;
	//EnterCriticalSection(&cs);//критическая секция
	std::unordered_map <LONGLONG, POINT> LifePointOld(LifePoint);
	//LeaveCriticalSection(&cs);//критическая секция
	std::unordered_map<LONGLONG, POINT>::iterator i;

	while (!CalcEnd) {

		for (i = LifePointOld.begin(); i != LifePointOld.end(); i++) //заполняем облако
		{
			for (int y = 0; y < 3; y++) //обработка 9 координат
			{
				for (int x = 0; x < 3; x++)
				{
					tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
					calc.Insert(tmpPoint, false, loudLifePoint);
				}
			}
		}

		for (i = loudLifePoint.begin(); i != loudLifePoint.end(); i++) //считаем новое поколение
		{
			for (int y = 0; y < 3; y++) //обработка 9 координат
			{
				for (int x = 0; x < 3; x++)
				{
					if (!(y == 1 && x == 1))
					{
						tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
						if (calc.Contains(tmpPoint, LifePointOld))	L++;
					}
				}
			}
			//EnterCriticalSection(&cs);//критическая секция
			if (L<2) calc.Insert(i->second, true, LifePoint);
			else if (L==3) calc.Insert(i->second, false, LifePoint);
			else if (L > 3) calc.Insert(i->second, true, LifePoint);
			//LeaveCriticalSection(&cs);//критическая секция
			L = 0;
		}
	//EnterCriticalSection(&cs);//критическая секция
	CalcEnd = true;
	//LeaveCriticalSection(&cs);//критическая секция

	}
}

void Calc::RunLife(bool &CalcEnd)

{
	//InitializeCriticalSection(&cs);
	std::vector<std::thread> thr(2); //для запуска двух потоков
	for (int i = 1; i <= 1; i++) thr[i - 1] = std::thread(Run, std::ref(CalcEnd), std::ref(this->LifePoint));
	for (int i = 1; i <= 1; i++) thr[i - 1].detach(); //отделяем потоки
	//for (int i = 1; i <= 1; i++) thr[i - 1].join(); //ждем все потоки

}

void Calc::DelLife()

{
	this->LifePoint.clear();
}

Calc::~Calc()
{
}


