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

//bool CalcEnd = true; //вычисления закончены - готов расчет нового поколения

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

bool Calc::Contains(Point point, std::unordered_map<LONGLONG, Point> &LifePoint) //аналог std::unordered_map::contains (C++20) //только живые
{
	std::unordered_map<LONGLONG, Point>::iterator i;
	i = LifePoint.find(HashPoint(point));
	if (i == LifePoint.end()) return false;
	return i->second.life;
}
void Calc::Insert(Point point, std::unordered_map <LONGLONG, Point> &LifePoint, bool pointDelete) //сОздаем для точки итератор
{
	Point pointTmp = { point.x,point.y};
	const LONGLONG hashPoint = HashPoint(pointTmp);
	std::unordered_map<LONGLONG, Point>::iterator i;
	i = LifePoint.find(hashPoint); //для удаляемой точки всегда должен быть не конечный итератор
	if (pointDelete) //если удаляем
	{
		if (i != LifePoint.end() && i->second.life==true) InsertRun(i, pointDelete); //удаляем если есть такая точка и она живая
	}
	else //если добавляем
	{
		if (i == LifePoint.end())//если такой точки нет - создаем
		{
			i = LifePoint.emplace(hashPoint, pointTmp).first;
			InsertRun(i, pointDelete);
		}
		else //если есть
		{
			if (i->second.life == false) InsertRun(i, pointDelete); //создаем если не было живой
		}
	}
}

void Calc::InsertRun(std::unordered_map<LONGLONG, Point>::iterator i, bool pointDelete) //расчет по итераторам
{
	long LifePointRunSizeTmp = LifePointRun.size(); //размер массива RUN
	int n[3] = { -1, 0, 1 };
	LONGLONG hashPointTmp;
	Point pointTmp;
	std::unordered_map<LONGLONG, Point>::iterator iTmp;

	if (pointDelete) //если удаляем точку
	{
		for (int y = 0; y < 3; y++) // проходим по 8 соседним точкам
		{
			for (int x = 0; x < 3; x++)
			{
				if (y == 1 && x == 1)//считаем саму себя
				{ 
					if (i != LifePoint.end()) // если такая точка есть (для удаляемой точки)
					{
						Population--;
						i->second.life = false;
						LifePointOut.erase(i->first);//удяляем в массиве для вывода
						i->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
						}
						if (i->second.update == 1)LifePointRun[++LifePointRunSize - 1] = i;
					}
				}
				else //считаем окружающие
				{
					pointTmp = { i->second.x + x - 1, i->second.y + y - 1 };
					hashPointTmp = HashPoint(pointTmp);
					iTmp = LifePoint.find(hashPointTmp);
					if (iTmp != LifePoint.end())
					{
						iTmp->second.state--;
						iTmp->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
						}
						if (iTmp->second.update==1)LifePointRun[++LifePointRunSize - 1] = iTmp;
					}
				}
			}
		}
	}
	else //если добавляем
	{
		for (int y = 0; y < 3; y++) // проходим по 8 соседним точкам
		{
			for (int x = 0; x < 3; x++)
			{
				if (y == 1 && x == 1)//считаем саму себя
				{
					Population++;
					i->second.life = true;
					LifePointOut.emplace(i->first, i->second);//добавляем в массиве для вывода
					i->second.update++;
					if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
					{
						LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
						LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
					}
					if (i->second.update == 1)LifePointRun[++LifePointRunSize - 1] = i;
				}
				else
				{	//считаем окружающие
					pointTmp = { i->second.x + x - 1, i->second.y + y - 1};
					hashPointTmp = HashPoint(pointTmp);
					iTmp = LifePoint.find(hashPointTmp);
					if (iTmp == LifePoint.end()) //если нет такой точки и добавляем её
					{
						//LifePoint.insert(iTmp, { hashPointTmp ,pointTmp });
						iTmp = LifePoint.emplace(hashPointTmp, pointTmp).first;
						iTmp->second.state++;
						iTmp->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
						}
						if (iTmp->second.update == 1)LifePointRun[++LifePointRunSize - 1] = iTmp;
					}
					else //если есть такая точка
					{
						iTmp->second.state++;
						iTmp->second.update++;
						if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
						}
						if (iTmp->second.update == 1)LifePointRun[++LifePointRunSize - 1] = iTmp;
					}

				}
			}
		}
		if (AreaXmin > i->second.x)AreaXmin = i->second.x;//расчет ареала 
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
	long LifePointRunSizeNew = LifePointRunNew.size(); //размер массива RUNtmp
	//long LifePointRunSizeOld = LifePointRunSize;
	long LifePointRunSizeTmp = 0;
	for (long j = 0; j < LifePointRunSize; j++) //считаем новое поколение по RUN
	{
		LifePointRun[j]->second.update = 0;
		if (LifePointRun[j]->second.state == 3 && LifePointRun[j]->second.life ==false)//добавляем
		{
			LifePointRun[j]->second.life = true;
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//добавляем в RUNnew массив
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //увеличиваем размер массива, если не хватает
			}
			LifePointRunNew[++LifePointRunSizeTmp-1]= LifePointRun[j];
		}
		else if((LifePointRun[j]->second.state > 3 || LifePointRun[j]->second.state < 2) && LifePointRun[j]->second.life == true)//удаляем
		{
			LifePointRun[j]->second.life = false;
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//добавляем в RUNnew массив
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //увеличиваем размер массива, если не хватает
			}
			LifePointRunNew[++LifePointRunSizeTmp - 1] = LifePointRun[j];
		}
	}

	LifePointRunSize = 0;
	for (long j = 0; j < LifePointRunSizeTmp; j++) //считаем новое поколение по RUN
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
	
		//удаляем пустые точки (если в массиве size больше 1 млн)
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
//	//сохраняем данные, запускаем новый расчет в отдельном потоке
//	Update();
//	CalcEnd = false;
//
//std::vector<std::thread> thr(1); //для запуска потоков
//for (int i = 1; i <= 1; i++) thr[i - 1] = std::thread(&Calc::RunLife, this);
////for (int i = 1; i <= 1; i++) thr[i - 1].detach(); //отделяем потоки
//for (int i = 1; i <= 1; i++) thr[i - 1].join(); //ждем все потоки
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


