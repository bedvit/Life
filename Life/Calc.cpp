#include "stdafx.h"
#include "Calc.h"
#include <unordered_map>
#include <map>
#include <thread>

#define SIZE_ARRAY 1000000

static long generation = 0;
//static long generationOld = 0;
//static long population = 0;
//static long areaXmin= LONG_MAX;
//static long areaYmin= LONG_MAX;
//static long areaXmax= LONG_MIN;
//static long areaYmax= LONG_MIN;
//static std::unordered_map <LONGLONG, Point> LifePoint;
static std::vector<std::unordered_map<LONGLONG, Point>::iterator> LifePointRun;
static std::vector<std::unordered_map<LONGLONG, Point>::iterator> LifePointRunNew;
//static std::unordered_map <LONGLONG, Point> LifePointO;
static long LifePointRunSize;
//static long LifePointRunSizeTmp;

//bool CalcEnd = true; //���������� ��������� - ����� ������ ������ ���������

//static std::unordered_map<LONGLONG, Point> LifePointRun;


Calc::Calc()
{
	AreaXmin = LLONG_MAX;// LLONG_MAX;
	AreaYmin = LONG_MAX;
	AreaXmax = LONG_MIN;
	AreaYmax = LONG_MIN;
	//Generation = generation;
	//Population = population;
	//AreaXmin = areaXmin;
	//AreaYmin = areaYmin;
	//AreaXmax = areaXmax;
	//AreaYmax = areaYmax;
	//CalcEnd = true;
}

LONGLONG Calc::HashPoint(Point point)
{
	return (LONGLONG)point.x * 2147483648 + point.y; //2^31
}

bool Calc::Contains(Point point, std::unordered_map<LONGLONG, Point> &LifePoint) //������ std::unordered_map::contains (C++20) //������ �����
{
	std::unordered_map<LONGLONG, Point>::iterator i;
	i = LifePoint.find(HashPoint(point));
	if (i == LifePoint.end()) return false;
	return i->second.life;
}
void Calc::Insert(Point point, std::unordered_map <LONGLONG, Point> &LifePoint, bool pointDelete) //������� ��� ����� ��������
{
	Point pointTmp = { point.x,point.y};
	const LONGLONG hashPoint = HashPoint(pointTmp);
	std::unordered_map<LONGLONG, Point>::iterator i;
	i = LifePoint.find(hashPoint); //��� ��������� ����� ������ ������ ���� �� �������� ��������
	if (pointDelete) //���� �������
	{
		if (i != LifePoint.end() && i->second.life==true) InsertRun(i, pointDelete); //������� ���� ���� ����� ����� � ��� �����
	}
	else //���� ���������
	{
		if (i == LifePoint.end())//���� ����� ����� ��� - �������
		{
			i = LifePoint.emplace(hashPoint, pointTmp).first;
			InsertRun(i, pointDelete);
		}
		else //���� ����
		{
			if (i->second.life == false) InsertRun(i, pointDelete); //������� ���� �� ���� �����
		}
	}
}

void Calc::InsertRun(std::unordered_map<LONGLONG, Point>::iterator i, bool pointDelete) //������ �� ����������
{
	long LifePointRunSizeTmp = LifePointRun.size(); //������ ������� RUN
	int n[3] = { -1, 0, 1 };
	LONGLONG hashPointTmp;
	Point pointTmp;
	std::unordered_map<LONGLONG, Point>::iterator iTmp;

	if (pointDelete) //���� ������� �����
	{
		for (int y = 0; y < 3; y++) // �������� �� 8 �������� ������
		{
			for (int x = 0; x < 3; x++)
			{
				if (y == 1 && x == 1)//������� ���� ����
				{ 
					if (i != LifePoint.end()) // ���� ����� ����� ���� (��� ��������� �����)
					{
						Population--;
						i->second.life = false;
						LifePointOut.erase(i->first);//������� � ������� ��� ������
						i->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
						}
						if (i->second.update == 1)LifePointRun[++LifePointRunSize - 1] = i;
					}
				}
				else //������� ����������
				{
					pointTmp = { i->second.x + x - 1, i->second.y + y - 1 };
					hashPointTmp = HashPoint(pointTmp);
					iTmp = LifePoint.find(hashPointTmp);
					if (iTmp != LifePoint.end())
					{
						iTmp->second.state--;
						iTmp->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
						}
						if (iTmp->second.update==1)LifePointRun[++LifePointRunSize - 1] = iTmp;
					}
				}
			}
		}
	}
	else //���� ���������
	{
		for (int y = 0; y < 3; y++) // �������� �� 8 �������� ������
		{
			for (int x = 0; x < 3; x++)
			{
				if (y == 1 && x == 1)//������� ���� ����
				{
					Population++;
					i->second.life = true;
					LifePointOut.emplace(i->first, i->second);//��������� � ������� ��� ������
					i->second.update++;
					if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
					{
						LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
						LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
					}
					if (i->second.update == 1)LifePointRun[++LifePointRunSize - 1] = i;
				}
				else
				{	//������� ����������
					pointTmp = { i->second.x + x - 1, i->second.y + y - 1};
					hashPointTmp = HashPoint(pointTmp);
					iTmp = LifePoint.find(hashPointTmp);
					if (iTmp == LifePoint.end()) //���� ��� ����� ����� � ��������� �
					{
						//LifePoint.insert(iTmp, { hashPointTmp ,pointTmp });
						iTmp = LifePoint.emplace(hashPointTmp, pointTmp).first;
						iTmp->second.state++;
						iTmp->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
						}
						if (iTmp->second.update == 1)LifePointRun[++LifePointRunSize - 1] = iTmp;
					}
					else //���� ���� ����� �����
					{
						iTmp->second.state++;
						iTmp->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
						}
						if (iTmp->second.update == 1)LifePointRun[++LifePointRunSize - 1] = iTmp;
					}

				}
			}
		}
		if (AreaXmin > i->second.x)AreaXmin = i->second.x;//������ ������ 
		if (AreaYmin > i->second.y)AreaYmin = i->second.y;
		if (AreaXmax < i->second.x)AreaXmax = i->second.x;
		if (AreaYmax < i->second.y)AreaYmax = i->second.y;
	}
}

//long Calc::Generation(){ return generation;}
//long Calc::AreaXmin() { return areaXmin; }
//long Calc::AreaYmin() { return areaYmin; }
//long Calc::AreaXmax() { return areaXmax; }
//long Calc::AreaYmax() { return areaYmax; }

void Calc::RunLife()
{
	long LifePointRunSizeNew = LifePointRunNew.size(); //������ ������� RUNtmp
	//long LifePointRunSizeOld = LifePointRunSize;
	long LifePointRunSizeTmp = 0;
	for (long j = 0; j < LifePointRunSize; j++) //������� ����� ��������� �� RUN
	{
		LifePointRun[j]->second.update = 0;
		if (LifePointRun[j]->second.state == 3 && LifePointRun[j]->second.life ==false)//���������
		{
			LifePointRun[j]->second.life = true;
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//��������� � RUNnew ������
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //����������� ������ �������, ���� �� �������
			}
			LifePointRunNew[++LifePointRunSizeTmp-1]= LifePointRun[j];
		}
		else if((LifePointRun[j]->second.state > 3 || LifePointRun[j]->second.state < 2) && LifePointRun[j]->second.life == true)//�������
		{
			LifePointRun[j]->second.life = false;
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//��������� � RUNnew ������
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //����������� ������ �������, ���� �� �������
			}
			LifePointRunNew[++LifePointRunSizeTmp - 1] = LifePointRun[j];
		}
	}

	LifePointRunSize = 0;
	for (long j = 0; j < LifePointRunSizeTmp; j++) //������� ����� ��������� �� RUN
	{
		if (LifePointRunNew[j]->second.life)
		{
			InsertRun(LifePointRunNew[j], false);
		}
		else if (!LifePointRunNew[j]->second.life)
		{
			InsertRun(LifePointRunNew[j], true);
		}
	}
	
		//������� ������ ����� (���� � ������� size ������ 1 ���)
	if (LifePoint.size() > SIZE_ARRAY)
	{
		std::vector<std::unordered_map<LONGLONG, Point>::iterator> NullPoint(LifePoint.size());
		long NullPointSize = 0;
		std::unordered_map<LONGLONG, Point>::iterator i;
		for (i = LifePoint.begin(); i != LifePoint.end(); i++)
		{
			if (i->second.state < 1 && i->second.life == false)
			{
				NullPoint[NullPointSize] = i;
				NullPointSize++;
			}
		}
		for (long j = 0; j < NullPointSize; j++)
		{
			LifePoint.erase(NullPoint[j]);
		}
	}
	++Generation;
	CalcEnd = true;
}

//void Calc::RunLifeTread()
//{
//	//while (!CalcEnd)
//	//{
//	//	int x = 0;
//	//}
//	//��������� ������, ��������� ����� ������ � ��������� ������
//	Update();
//	CalcEnd = false;
//
//std::vector<std::thread> thr(1); //��� ������� �������
//for (int i = 1; i <= 1; i++) thr[i - 1] = std::thread(&Calc::RunLife, this);
////for (int i = 1; i <= 1; i++) thr[i - 1].detach(); //�������� ������
//for (int i = 1; i <= 1; i++) thr[i - 1].join(); //���� ��� ������
//}

//void Calc::Update()
//{
//	Generation = generation;
//	Population = population;
//	AreaXmin = areaXmin;
//	AreaYmin = areaYmin;
//	AreaXmax = areaXmax;
//	AreaYmax = areaYmax;
//	//LifePointOut = LifePointO;
//}
void Calc::RunLifeStep(long& step)
{
	long stepTmp = 0;
	while(step> stepTmp)
	{
		RunLife();
		stepTmp++;
	}
}

void Calc::DelLife()
{
	LifePoint.clear();
	LifePointOut.clear();
	Generation = 0;
	Population = 0;
	LifePointRunSize = 0;
	//LifePointRunSizeTmp = 0;
	AreaXmin = LONG_MAX;
	AreaYmin = LONG_MAX;
	AreaXmax = LONG_MIN;
	AreaYmax = LONG_MIN;
}

Calc::~Calc()
{
}


