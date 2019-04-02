#include "stdafx.h"
#include "Calc.h"
#include <vector>

#define SIZE_ARRAY 10000000

static long generation = 0;
static std::vector<std::unordered_map<LONGLONG, unsigned char>::iterator> LifePointRun;
static std::vector<std::unordered_map<LONGLONG, unsigned char>::iterator> LifePointRunNew;
static long LifePointRunSize;

Calc::Calc()
{
	AreaXmin = LONG_MAX;
	AreaYmin = LONG_MAX;
	AreaXmax = LONG_MIN;
	AreaYmax = LONG_MIN;
	LifePoint.reserve(SIZE_ARRAY);
	LifePoint.max_load_factor(0.5);
}

union ULL 
{
	LONGLONG U;
	long L[2];
};

LONGLONG Calc::HashPoint(Point point)
{
	ULL ull;
	ull.L[0] = point.x;
	ull.L[1] = point.y;
	return ull.U;
}

bool Calc::Contains(Point point, std::unordered_map<LONGLONG, unsigned char> &LifePoint) //аналог std::unordered_map::contains (C++20) //только живые
{
	std::unordered_map<LONGLONG, unsigned char>::iterator i;
	i = LifePoint.find(HashPoint(point));
	if (i == LifePoint.end()) return false;
	return ((bool)((i->second >> 6) & 1));
}
void Calc::Insert(Point point, std::unordered_map<LONGLONG, unsigned char> &LifePoint, bool pointDelete, Grid& grid) //сОздаем для точки итератор
{
	LONGLONG hashPoint = HashPoint({ point.x,point.y });
	std::unordered_map<LONGLONG, unsigned char>::iterator i;
	i = LifePoint.find(hashPoint); //для удаляемой точки всегда должен быть не конечный итератор
	if (pointDelete) //если удаляем
	{
		if (i != LifePoint.end() && ((bool)((i->second >> 6) & 1))) InsertRun(i, pointDelete, grid); //удаляем если есть такая точка и она живая
	}
	else //если добавляем
	{
		if (i == LifePoint.end())//если такой точки нет - создаем
		{
			i = LifePoint.emplace(hashPoint, 144).first;
			InsertRun(i, pointDelete, grid);
		}
		else //если уже есть живая
		{
			if (!(bool)((i->second >> 6) & 1)) InsertRun(i, pointDelete, grid); //создаем если не было живой//3й байт с начала, 6й с конца - (little-endian)
		}
	}
}

void Calc::InsertRun(std::unordered_map<LONGLONG, unsigned char>::iterator i, bool pointDelete, Grid& grid) //расчет по итераторам
{
	long LifePointRunSizeTmp = LifePointRun.size(); //размер массива RUN
	int n[3] = { -1, 0, 1 };
	LONGLONG hashPointTmp;
	std::unordered_map<LONGLONG, unsigned char>::iterator iTmp;
	ULL ull;
	ull.U = i->first;

	if (pointDelete) //если удаляем точку
	{
		for (int y = 0; y < 3; ++y) // проходим по 8 соседним точкам
		{
			for (int x = 0; x < 3; ++x)
			{
				if (y == 1 && x == 1)//считаем саму себя
				{ 
						--Population;
						i->second &= ~(1 << 6);//выключаем 7й бит
						if (!grid.updateBuffer) //рисуем точку если выключено обновление буфера
						{
							grid.DrawPoint(i);
						}
						
						if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
						}
						if (!(bool)((i->second >> 5) & 1))LifePointRun[++LifePointRunSize - 1] = i;
						i->second |= 1 << 5; //включаем 6 бит
				}
				else //считаем окружающие
				{
					iTmp = LifePoint.find(HashPoint({ ull.L[0] + x - 1, ull.L[1] + y - 1 }));
						--iTmp->second;
						if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
						}
						if (!(bool)((iTmp->second >> 5) & 1))LifePointRun[++LifePointRunSize - 1] = iTmp;
						iTmp->second |= 1 << 5; //включаем 6 бит
				}
			}
		}
	}
	else //если добавляем
	{
		for (int y = 0; y < 3; ++y) // проходим по 8 соседним точкам
		{
			for (int x = 0; x < 3; ++x)
			{
				if (y == 1 && x == 1)//считаем саму себя
				{
					++Population;
					i->second |= 1 << 6; //включаем 7й бит
					if (!grid.updateBuffer) //рисуем точку если выключено обновление буфера
					{
						grid.DrawPoint(i);
					}
					if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
					{
						LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
						LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
					}
					if (((i->second >> 5) & 1) == 0)LifePointRun[++LifePointRunSize - 1] = i;
					i->second |= 1 << 5;//включаем 6 бит				
				}
				else
				{	//считаем окружающие
					iTmp = LifePoint.try_emplace(HashPoint({ ull.L[0] + x - 1, ull.L[1] + y - 1 }), 144).first;
					++iTmp->second;
					if (LifePointRunSize >= LifePointRunSizeTmp)//добавляем в RUN массив
					{
						LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
						LifePointRun.resize(LifePointRunSizeTmp); //увеличиваем размер массива, если не хватает
					}
					if (!(bool)((iTmp->second >> 5) & 1))LifePointRun[++LifePointRunSize - 1] = iTmp;
					iTmp->second |= 1 << 5;//включаем 6 бит
				}
			}
		}
		if (AreaXmin > ull.L[0])AreaXmin = ull.L[0];//расчет ареала 
		if (AreaYmin > ull.L[1])AreaYmin = ull.L[1];
		if (AreaXmax < ull.L[0])AreaXmax = ull.L[0];
		if (AreaYmax < ull.L[1])AreaYmax = ull.L[1];
	}
}

void Calc::RunLife(Grid& grid)
{
	long LifePointRunSizeNew = LifePointRunNew.size(); //размер массива RUNtmp
	long LifePointRunSizeTmp = 0;
	for (long j = 0; j < LifePointRunSize; ++j) //считаем новое поколение по RUN
	{
		LifePointRun[j]->second &= 223;//выключаем 6й бит (&11111011) - отключаем обновление
		unsigned char ss = LifePointRun[j]->second & 15; //(&11110000)
		if (ss == 3 && (!(bool)((LifePointRun[j]->second >> 6) & 1)))//добавляем если 3 точки вокруг
		{
			LifePointRun[j]->second |= 1 << 6; //включаем 7 бит (жизнь)
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//добавляем в RUNnew массив
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //увеличиваем размер массива, если не хватает
			}
			LifePointRunNew[++LifePointRunSizeTmp-1]= LifePointRun[j];
		}
		else if (ss < 1 && (!(bool)((LifePointRun[j]->second >> 6) & 1)))//удаляем пустые ячейки
		{
			LifePoint.erase(LifePointRun[j]);
		}
		else if((ss > 3 || ss < 2) && ((bool)((LifePointRun[j]->second >> 6) & 1)))//удаляем
		{
			LifePointRun[j]->second &= ~(1 << 6);//LifePointRun[j]->second.life = false;
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//добавляем в RUNnew массив
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //увеличиваем размер массива, если не хватает
			}
			LifePointRunNew[++LifePointRunSizeTmp - 1] = LifePointRun[j];
		}
	}

	LifePointRunSize = 0;
	for (long j = 0; j < LifePointRunSizeTmp; ++j) //считаем новое поколение по RUN
	{
		if ((bool)((LifePointRunNew[j]->second >> 6) & 1))
		{
			InsertRun(LifePointRunNew[j], false, grid);
		}
		else if (!(bool)((LifePointRunNew[j]->second >> 6) & 1))
		{
			InsertRun(LifePointRunNew[j], true, grid);
		}
	}
	++Generation;
}

void Calc::RunLifeStep(long step, Grid& grid)
{
	long stepTmp = 0;
	while(step> stepTmp)
	{
		RunLife(grid);
		++stepTmp;
	}
}

void Calc::DelLife()
{
	LifePoint.clear();
	Generation = 0;
	Population = 0;
	LifePointRunSize = 0;
	AreaXmin = LONG_MAX;
	AreaYmin = LONG_MAX;
	AreaXmax = LONG_MIN;
	AreaYmax = LONG_MIN;
}

Calc::~Calc()
{
}


