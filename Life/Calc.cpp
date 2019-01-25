#include "stdafx.h"
#include "Calc.h"
#include <unordered_map>
#include <map>
//#include <thread>
//#include "windows.h"
//#include "dos.h"
//#include <deque>
//#include <concurrent_unordered_map.h>
//#include <ppl.h>
//#include <array>
//#include <sstream>
//#include <iostream>
//
//#include <algorithm>
//#include <string>
//#include <vector>
//#include <numeric>


//CRITICAL_SECTION cs;//����������� ������


static long generation=0;
static long areaXmin= LONG_MAX;
static long areaYmin= LONG_MAX;
static long areaXmax= LONG_MIN;
static long areaYmax= LONG_MIN;
//static std::deque<POINT> loudLifePoint;//������
//static std::vector<POINT> loudLifePoint;//������

//static std::unordered_map <LONGLONG, POINT> LifePointRun; //���������� ���������
static std::unordered_map<LONGLONG, POINT> LifePointRun;
////static std::unordered_map<LONGLONG, POINT> LifePointRunOld;
////static std::unordered_map <LONGLONG, POINT> LifePointNew;
//static std::unordered_map <LONGLONG, POINT> LifePointOld;
//static std::unordered_map <LONGLONG, POINT> LifePointLoud;
//static std::unordered_map <LONGLONG, POINT> LifePointTmp;
//static std::unordered_map <LONGLONG, POINT> LifePoint;


//#include <ppl.h>
//#include <concurrent_unordered_map.h>
//#include <iostream>

//using namespace concurrency;
//using namespace std;

Calc::Calc()
{
	//Generation2= generation;
	//LifePoint;

}
//std::unordered_map& Calc::LifePoint()
//{
//	LifePoint= LifePoint
//}

LONGLONG Calc::HashPoint(POINT point)
{
	return (LONGLONG)point.x * 1000000000 + point.y;
}

//void InsertRun(POINT point, bool invert)
//
//{
//	Calc calc;
//	//EnterCriticalSection(&cs);//����������� ������
//	LONGLONG tmp = calc.HashPoint(point);
//	if (invert)LifePointRun.erase(tmp); else LifePointRun[tmp] = point;
//	//LeaveCriticalSection(&cs);//����������� ������
//}

void Calc::Insert(POINT point, std::unordered_map <LONGLONG, POINT> &LifePoint, bool invert)

{
	//EnterCriticalSection(&cs);//����������� ������
	LONGLONG tmp = HashPoint(point);
	if (invert)
	{
		//EnterCriticalSection(&cs);//����������� ������
		LifePoint.erase(tmp); // �� ������������ �����������
		LifePointRun.erase(tmp); // �� ������������ �����������
		//LeaveCriticalSection(&cs);//����������� ������
	}
	else
	{
		LifePoint[tmp] = point;
		LifePointRun[tmp] = point;
		if (areaXmin > point.x)areaXmin = point.x;//������ ������ 
		if (areaYmin > point.y)areaYmin = point.y;
		if (areaXmax < point.x)areaXmax = point.x;
		if (areaYmax < point.y)areaYmax = point.y;
	}
//LeaveCriticalSection(&cs);//����������� ������
}

long Calc::Generation(){ return generation;}
long Calc::AreaXmin() { return areaXmin; }
long Calc::AreaYmin() { return areaYmin; }
long Calc::AreaXmax() { return areaXmax; }
long Calc::AreaYmax() { return areaYmax; }


//bool& Calc::Pause() { return pause; }

bool Calc::Contains(POINT point, std::unordered_map<LONGLONG, POINT> &LifePoint) //������ std::unordered_map::contains (C++20)

{
	LONGLONG tmp = HashPoint(point);
	if (LifePoint.count(tmp) == 1) return true; else return false;

}


//void Run(long t, long treads)
//{
//
//	//while (RunCalc && !CalcEnd) {
//	//	if (!Pause)
//	//	{
//	int n[3] = { -1, 0, 1 };
//	POINT tmpPoint;// = { 0,0 };
//	Calc calc;
//
//	//long iter;
//	//std::unordered_map <LONGLONG, POINT> LifePointLoud; //������
//	//EnterCriticalSection(&cs);//����������� ������
//	//std::unordered_map <LONGLONG, POINT> LifePointOld(LifePoint); //���������� ���������
//	//LeaveCriticalSection(&cs);//����������� ������
//	
//
//
//	//if (generation==0)
//	//{
//	//	LifePointRunOld = LifePointOld;
//	//}
//	POINT x = LifePointRunOld[0];
//	x = LifePointRunOld[1];
//	//concurrency::parallel_for(LifePointRunOld.begin(); i != LifePointRunOld.end(); i++) //��������� ������
//	long end = LifePointRunOld.size();
//	std::unordered_map<LONGLONG, POINT>::iterator i;
//	for (i = LifePointRunOld.begin(); i != LifePointRunOld.end(); std::advance(i,2)) //��������� ������
//	//for (long i=0; i< end; i++) //��������� ������
//	{
//		for (int y = 0; y < 3; y++) //��������� 9 ���������
//		{
//			for (int x = 0; x < 3; x++)
//			{
//				tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
//				//tmpPoint = { LifePointRunOld[i].x + n[x], LifePointRunOld[i].y + n[y] };
//				//tmpPoint = LifePointRunOld[i];
//				calc.Insert(tmpPoint, LifePointLoud, false);
//				//loudLifePoint[iter] = tmpPoint;
//				//iter++;
//			}
//		}
//	}
//}
//
//void RunNext(std::unordered_map <LONGLONG, POINT> &LifePoint, long t, long treads)
//{
//			int n[3] = { -1, 0, 1 };
//			POINT tmpPoint;// = { 0,0 };
//			Calc calc;
//			int L = 0;
//			std::unordered_map<LONGLONG, POINT>::iterator i;
//			for (i = LifePointLoud.begin(); i != LifePointLoud.end(); i++) //������� ����� ��������� �� ������
//			{
//				for (int y = 0; y < 3; y++) //��������� 9 ���������
//				{
//					for (int x = 0; x < 3; x++)
//					{
//						if (!(y == 1 && x == 1))
//						{
//							
//							
//								tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
//								if (calc.Contains(tmpPoint, LifePointOld))	L++;
//								if (L > 3) goto nx_; //��� ������ ������� ����� 4 ����� ������
//						}
//					}
//				}
//				//EnterCriticalSection(&cs);//����������� ������
//				nx_:
//				if (L < 2)
//				{
//					if (calc.Contains(i->second, LifePointOld)) //���� ����� ����
//					{
//						calc.Insert(i->second, LifePoint, true);
//						calc.Insert(i->second, LifePointRun, false);
//					}
//				}
//				else if (L == 2)
//				{
//					
//				}
//				else if (L == 3)
//				{
//					if (!calc.Contains(i->second, LifePointOld)) //���� ����� �� ����
//					{
//						calc.Insert(i->second, LifePoint, false);
//						calc.Insert(i->second, LifePointRun, false);
//					}
//				}
//				else if (L > 3)
//				{
//					if (calc.Contains(i->second, LifePointOld)) //���� ����� ����
//					{
//						calc.Insert(i->second, LifePoint, true);
//						calc.Insert(i->second, LifePointRun, false);
//					}
//				}
//				//LeaveCriticalSection(&cs);//����������� ������
//				L = 0;
//			}
//			//EnterCriticalSection(&cs);//����������� ������
//			//++generation;
//			//CalcEnd = true;
//			//LeaveCriticalSection(&cs);//����������� ������
//		//}
//	//}
//}

void Calc::RunLife()

{
		//InitializeCriticalSection(&cs);
		//long treads = std::thread::hardware_concurrency();//���������� ����� ������������ ����������� �������, �������������� �����������
		Calc calc;
		long L = 0;
		POINT tmpPoint;
		//std::unordered_map<LONGLONG, POINT> LifePointRun;
		//static std::unordered_map<LONGLONG, POINT> LifePointRunOld;
		//static std::unordered_map <LONGLONG, POINT> LifePointNew;
		std::unordered_map <LONGLONG, POINT> LifePointOld;
		std::unordered_map <LONGLONG, POINT> LifePointLoud;
		//LifePointOld = LifePoint;

		//if (generation==0)
		//{
		//	LifePointRunOld = LifePoint;
		//}
		//else
		//{
		//	LifePointRunOld = LifePointRun;
		//}
		//LifePointRun.clear();
		//LifePointLoud.clear();
		//std::unordered_map<LONGLONG, POINT>::iterator i;
		//i = LifePointRunOld.begin(); 
		//i = LifePointRunOld.end();


		//std::vector<std::thread> thr(treads); //��� ������� �������
		//for (int i = 1; i <= treads; i++) thr[i - 1] = std::thread(Run, i - 1, treads);
		////for (int i = 1; i <= 1; i++) thr[i - 1].detach(); //�������� ������
		//for (int i = 1; i <= treads; i++) thr[i - 1].join(); //���� ��� ������

		//for (int i = 1; i <= treads; i++) thr[i - 1] = std::thread(RunNext, std::ref(this->LifePoint),i-1, treads);
		////for (int i = 1; i <= 1; i++) thr[i - 1].detach(); //�������� ������
		//for (int i = 1; i <= treads; i++) thr[i - 1].join(); //���� ��� ������
		//LifePointOld = LifePoint;
		if (generation==0)	LifePointRun = LifePoint;
		//LifePointOld = LifePoint;
		std::unordered_map<LONGLONG, POINT>::iterator i;
		for (i = LifePointRun.begin(); i != LifePointRun.end(); i++) //������� ����� ��������� �� Run - �������
		{
			for (int y = 0; y < 3; y++) //��������� 9 ���������
			{
				for (int x = 0; x < 3; x++)
				{
					int n[3] = { -1, 0, 1 };
					tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
					//LifePointLoud.erase(HashPoint(tmpPoint));
					LifePointLoud[HashPoint(tmpPoint)] = tmpPoint; //��������
					//calc.Insert(tmpPoint, LifePointLoud, false);

				}
			}
		}

		LifePointRun.clear();
		//LifePointNew=LifePoint;
		LifePointOld = LifePoint;
		for (i = LifePointLoud.begin(); i != LifePointLoud.end(); i++) //������� ����� ��������� �� ������
		{

			for (int y = 0; y < 3; y++) //��������� 9 ���������
			{
				for (int x = 0; x < 3; x++)
				{
					if (!(y == 1 && x == 1))
					{
						int n[3] = { -1, 0, 1 };
						tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
						if (calc.Contains(tmpPoint, LifePointOld)) L++;
						if (L > 3) goto nx_; //��� ������ ������� ����� 4 ����� ������
					}
				}
			}

			//EnterCriticalSection(&cs);//����������� ������
		nx_:
			tmpPoint = i->second;
			if (L < 2|| L > 3)
			{
				if (calc.Contains(tmpPoint, LifePointOld)) //���� ����� ����
				{
					//calc.Insert(i->second, LifePoint, true);
					//calc.Insert(i->second, LifePointRun, false);
					LifePoint.erase(HashPoint(tmpPoint)); //�������
					LifePointRun[HashPoint(tmpPoint)] = tmpPoint; //��������
				}
			}
			else if (L == 3)
			{
				if (!calc.Contains(tmpPoint, LifePointOld)) //���� ����� �� ����
				{
					//calc.Insert(i->second, LifePoint, false);
					//calc.Insert(i->second, LifePointRun, false);
					//LifePoint[HashPoint(tmpPoint)] = tmpPoint; //��������
					//LifePointRun[HashPoint(tmpPoint)] = tmpPoint; //��������
					calc.Insert(tmpPoint, LifePoint, false);
				}
			}

			//LeaveCriticalSection(&cs);//����������� ������
			L = 0;

		}
		//LifePoint=LifePointNew;
		//LifePoint = LifePointTmp;
		//long cz = LifePoint.size();
		//long cz2 = LifePoint.size();
		//long cz3 = LifePoint.size();
		++generation;
		//DeleteCriticalSection(&cs);
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


