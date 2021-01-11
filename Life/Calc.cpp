#include "stdafx.h"
#include "Calc.h"
#include <vector>
#include <utility>

#define SIZE_ARRAY 1000000
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> Run;
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> Run1; //массив с живими
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> Run2; //массив с гибнущими
static long RunSize;

Calc::Calc()
{
	AreaXmin = LONG_MAX;
	AreaYmin = LONG_MAX;
	AreaXmax = LONG_MIN;
	AreaYmax = LONG_MIN;
	LifePoint.reserve(SIZE_LIFEPOINT);
	LifePointSize = 0;
	//LifePoint.max_load_factor(0.5);
}

union ULL 
{
	LONGLONG U;
	long L[2];
};

union IC
{
	std::unordered_map<LONGLONG, unsigned char[SIZE_POINT]>::iterator* I;
	unsigned char C[SIZE_POINT];
};

LONGLONG Calc::HashPoint(POINT point)
{
	ULL ull;
	ull.L[0] = point.x;
	ull.L[1] = point.y;
	return ull.U;
}

bool Calc::Contains(POINT point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint) //аналог std::unordered_map::contains (C++20) //только живые
{
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	i = LifePoint.find(HashPoint(point));
	if (i == LifePoint.end()) return false;
	return (i->second[SIZE_POINT - 2] == 1);
}

int Calc::Insert(POINT point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint, bool pointDelete, Grid& grid) //сОздаем для точки итератор
{
	Msg msg;
	if (LifePointSize >= SIZE_LIFEPOINT) //Overflow Life
	{
		msg.ErrOverflow();
		return 1;
	}
	
	LONGLONG hashPoint = HashPoint({ point.x,point.y });
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	i = LifePoint.find(hashPoint); //для удаляемой точки всегда должен быть не конечный итератор
	if (pointDelete) //если удаляем
	{
		if (i != LifePoint.end() && (i->second[SIZE_POINT - 2] == 1))//удаляем если есть такая точка и она живая
		{
			InsertRun(i, pointDelete, grid); 
		}
	}
	else //если добавляем
	{
		if (i == LifePoint.end())//если такой точки нет - создаем
		{
			//unsigned char [SIZE_POINT] - первые 4/8 байт (для х32), т.е. 1-4 байты - указатель на массив итераторов (облако), далее байты - состояние точки.
			//обратный порядок байт - (little-endian)
			//LifePoint.second[SIZE_POINT - 4] - 5й байт (нумерация с 0) - не используется
			//LifePoint.second[SIZE_POINT - 3] - 6й байт - признак обновления	
			//LifePoint.second[SIZE_POINT - 2] - 7й байт - жизнь
			//LifePoint.second[SIZE_POINT - 1] - 8й байт - количество клеток вокруг (соседних)

			//i = LifePoint.emplace(hashPoint).first;
			i = LifePoint.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(hashPoint),
				std::forward_as_tuple()
			).first;
			++LifePointSize;
			i->second[SIZE_POINT - 1] = 0;
			if(InsertRun(i, pointDelete, grid)) return 1;
		}
		else //если уже есть 
		{
			if (i->second[SIZE_POINT - 2] == 0) //если не живая - обрабатываем
			{
				if (InsertRun(i, pointDelete, grid)) return 1;
			}
		}
	}
	return 0;
}

int Calc::InsertRun(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i, bool pointDelete, Grid& grid) //расчет по итераторам
{
	Msg msg;
	long RunSizeTmp = Run.size(); //размер массива RUN
	int n[3] = { -1, 0, 1 };
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator iTmp;
	ULL ull;
	ull.U = i->first;
	IC ic;
	memcpy(ic.C, i->second, SIZE_POINT);
	int yx = 0;
	if (pointDelete) //если удаляем точку
	{
		--Population;
		if (RunSize >= RunSizeTmp)//добавляем в RUN массив
		{
			RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
			Run.resize(RunSizeTmp); //увеличиваем размер массива, если не хватает
		}
		if (i->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = i;
		i->second[SIZE_POINT - 3] = 1;//включаем 6 байт

		for (int y = 0; y < 8; ++y) // проходим по 8 соседним точкам
		{
			iTmp = ic.I[y];
			--iTmp->second[SIZE_POINT-1];
			if (RunSize >= RunSizeTmp)//добавляем в RUN массив
			{
				RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
				Run.resize(RunSizeTmp); //увеличиваем размер массива, если не хватает
			}
			if (iTmp->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = iTmp;
			iTmp->second[SIZE_POINT - 3] = 1;//включаем 6 байт

		}
		delete[] ic.I; //удаляем облако 
		i->second[SIZE_POINT - 2] = 0;//выключаем 7й байт
		if (!grid.updateBuffer) //рисуем точку если выключено обновление буфера
		{
			grid.DrawPoint(i);
		}
	}
	else //если добавляем
	{
		ic.I = new std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator[8];//добавляем облако
		memcpy(i->second,ic.C,  SIZE_POINT);
		++Population;
		i->second[SIZE_POINT - 2] = 1;//включаем 7й байт
		if (!grid.updateBuffer) //рисуем точку если выключено обновление буфера
		{
			grid.DrawPoint(i);
		}
		if (RunSize >= RunSizeTmp)//добавляем в RUN массив
		{
			RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
			Run.resize(RunSizeTmp); //увеличиваем размер массива, если не хватает
		}
		if (i->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = i;
		i->second[SIZE_POINT - 3] = 1;//включаем 6 байт	

		for (int y = 0; y < 3; ++y) // проходим по 8 соседним точкам
		{
			for (int x = 0; x < 3; ++x)
			{
				if (y == 1 && x == 1)//считаем саму себя
				{
				}
				else
				{	//считаем окружающие
					if (LifePointSize >= SIZE_LIFEPOINT) //Overflow Life
					{
						msg.ErrOverflow();
						return 1;
					}
					iTmp = LifePoint.try_emplace(LifePoint.end(), HashPoint({ ull.L[0] + x - 1, ull.L[1] + y - 1 }));//!!
					++iTmp->second[SIZE_POINT-1] ;
					if (RunSize >= RunSizeTmp)//добавляем в RUN массив
					{
						RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
						Run.resize(RunSizeTmp); //увеличиваем размер массива, если не хватает
					}
					if (iTmp->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = iTmp;//включаем обновление

					iTmp->second[SIZE_POINT - 3] = 1;//включаем 6 байт
					////////////заполняем итераторами облако/////////////
					ic.I[yx] = iTmp;
					++yx;
				}
			}
		}
		if (AreaXmin > ull.L[0])AreaXmin = ull.L[0];//расчет ареала 
		if (AreaYmin > ull.L[1])AreaYmin = ull.L[1];
		if (AreaXmax < ull.L[0])AreaXmax = ull.L[0];
		if (AreaYmax < ull.L[1])AreaYmax = ull.L[1];
	}
	return 0;
}

int Calc::RunLife(Grid& grid)
{
	long RunSize1 = 0; //размер массива RUN1
	long RunSize2 = 0; //размер массива RUN2
	long RunSizeTmp1 = Run1.size(); //размер массива RUNtmp1
	long RunSizeTmp2 = Run2.size(); //размер массива RUNtmp2
	for (long j = 0; j < RunSize; ++j) //считаем новое поколение по RUN
	{
		Run[j]->second[SIZE_POINT - 3] = 0; //выключаем 6й байт - отключаем обновление
		unsigned char ss = Run[j]->second[SIZE_POINT - 1]; //количество клеток вокруг (соседних)
		if (ss == 3 && (Run[j]->second[SIZE_POINT - 2] == 0))//добавляем если 3 точки вокруг
		{
			if (RunSize1 >= RunSizeTmp1)//добавляем в RUN массив
			{
				RunSizeTmp1 = RunSizeTmp1 + SIZE_ARRAY;
				Run1.resize(RunSizeTmp1); //увеличиваем размер массива, если не хватает
			}
			Run1[++RunSize1-1]= Run[j];
		}
		else if (ss < 1 && (Run[j]-> second[SIZE_POINT - 2] == 0))//удаляем пустые ячейки
		{
			LifePoint.erase(Run[j]);
			--LifePointSize;
		}
		else if((ss > 3 || ss < 2) && (Run[j]->second[SIZE_POINT - 2] == 1))//удаляем
		{
			if (RunSize2 >= RunSizeTmp2)//добавляем в RUN массив
			{
				RunSizeTmp2 = RunSizeTmp2 + SIZE_ARRAY;
				Run2.resize(RunSizeTmp2); //увеличиваем размер массива, если не хватает
			}
			Run2[++RunSize2 - 1] = Run[j];
		}
	}

	RunSize = 0; //обнуляем массив RUN - меняющиеся ячейки
	for (long j = 0; j < RunSize1; ++j) //живые
	{
		if (InsertRun(Run1[j], false, grid)) return 1;
	}

	for (long j = 0; j < RunSize2; ++j) //гибнующие
	{
		if (InsertRun(Run2[j], true, grid)) return 1;
	}

	++Generation;
	return 0;
}

void Calc::RunLifeStep(ULONGLONG step, Grid& grid)
{
	ULONGLONG stepTmp = 0;
	while(step> stepTmp)
	{
		if (RunLife(grid)) return ;
		++stepTmp;
	}
}

void Calc::DelLife()
{
	//удаляем облака
	IC ic;
	long x=0;
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	for (i = LifePoint.begin(); i != LifePoint.end(); ++i)
	{
		x++;
		if (i->second[SIZE_POINT - 2] == 1)//если живой
		{
			memcpy(ic.C, i->second, SIZE_POINT);
			delete[] ic.I; //удаляем облако 
		}
	}

	LifePoint.clear();
	Generation = 0;
	Population = 0;
	RunSize = 0;
	AreaXmin = LONG_MAX;
	AreaYmin = LONG_MAX;
	AreaXmax = LONG_MIN;
	AreaYmax = LONG_MIN;
	LifePointSize=0;
}

Calc::~Calc()
{
}


