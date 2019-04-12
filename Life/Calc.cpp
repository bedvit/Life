#include "stdafx.h"
#include "Calc.h"
#include <vector>
#include <utility>

#define SIZE_ARRAY 1000000
static long generation = 0;
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> Run;
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> RunNew;
static long RunSize;

Calc::Calc()
{
	AreaXmin = LONG_MAX;
	AreaYmin = LONG_MAX;
	AreaXmax = LONG_MIN;
	AreaYmax = LONG_MIN;
	LifePoint.reserve(33000000);
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
	char C[SIZE_POINT];
};

LONGLONG Calc::HashPoint(Point point)
{
	ULL ull;
	ull.L[0] = point.x;
	ull.L[1] = point.y;
	return ull.U;
}

bool Calc::Contains(Point point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint) //������ std::unordered_map::contains (C++20) //������ �����
{
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	i = LifePoint.find(HashPoint(point));
	if (i == LifePoint.end()) return false;
	return (((i->second[SIZE_POINT - 1] >> 6) & 1) == 1);
}

void Calc::Insert(Point point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint, bool pointDelete, Grid& grid) //������� ��� ����� ��������
{
	LONGLONG hashPoint = HashPoint({ point.x,point.y });
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	i = LifePoint.find(hashPoint); //��� ��������� ����� ������ ������ ���� �� �������� ��������
	if (pointDelete) //���� �������
	{
		if (i != LifePoint.end() && (((i->second[SIZE_POINT-1]  >> 6) & 1) == 1))//������� ���� ���� ����� ����� � ��� �����
		{
			InsertRun(i, pointDelete, grid); 
		}
	}
	else //���� ���������
	{
		if (i == LifePoint.end())//���� ����� ����� ��� - �������
		{
			//unsigned char [SIZE_POINT] - ������ 4/8 ���� ��������� �� ������ ���������� (������), ��������� ���� ��������� �����.
			//�������� ������� ���� - (little-endian)
			//(144)10010000->00001001 - ����� �����
			//(176)10110000->00001_1_01 - 6� ��� ������� ����������	
			//(208)11010000->000010_1_1 - 7� ��� �����

			//i = LifePoint.emplace(hashPoint).first;
			i = LifePoint.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(hashPoint),
				std::forward_as_tuple()
			).first;
			i->second[SIZE_POINT - 1] = 144;
			InsertRun(i, pointDelete, grid);
			
		}
		else //���� ��� ���� 
		{
			if (((i->second[SIZE_POINT-1]  >> 6) & 1) == 0) //���� �� ����� - ������������
			{
				InsertRun(i, pointDelete, grid);
			}
		}
	}
}

void Calc::InsertRun(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i, bool pointDelete, Grid& grid) //������ �� ����������
{
	long RunSizeTmp = Run.size(); //������ ������� RUN
	int n[3] = { -1, 0, 1 };
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator iTmp;
	ULL ull;
	ull.U = i->first;
	IC ic;
	memcpy(ic.C, i->second, SIZE_POINT);
	int yx = 0;
	if (pointDelete) //���� ������� �����
	{
		--Population;
		i->second[SIZE_POINT-1]  &= ~(1 << 6);//��������� 7� ���
		if (!grid.updateBuffer) //������ ����� ���� ��������� ���������� ������
		{
			grid.DrawPoint(i);
		}
						
		if (RunSize >= RunSizeTmp)//��������� � RUN ������
		{
			RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
			Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
		}
		if (((i->second[SIZE_POINT-1]  >> 5) & 1) == 0)Run[++RunSize - 1] = i;
		i->second[SIZE_POINT-1]  |= 1 << 5; //�������� 6 ���

		for (int y = 0; y < 8; ++y) // �������� �� 8 �������� ������
		{
			iTmp = ic.I[y];
			--iTmp->second[SIZE_POINT-1];
			if (RunSize >= RunSizeTmp)//��������� � RUN ������
			{
				RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
				Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
			}
			if (((iTmp->second[SIZE_POINT-1]  >> 5) & 1) == 0)Run[++RunSize - 1] = iTmp;
			iTmp->second[SIZE_POINT-1]  |= 1 << 5; //�������� 6 ���

		}
		delete[] ic.I; //������� ������ 
	}
	else //���� ���������
	{
		ic.I = new std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator[8];//��������� ������
		memcpy(i->second,ic.C,  SIZE_POINT);

		for (int y = 0; y < 3; ++y) // �������� �� 8 �������� ������
		{
			for (int x = 0; x < 3; ++x)
			{
				if (y == 1 && x == 1)//������� ���� ����
				{
					++Population;
					i->second[SIZE_POINT-1]  |= 1 << 6; //�������� 7� ���
					if (!grid.updateBuffer) //������ ����� ���� ��������� ���������� ������
					{
						grid.DrawPoint(i);
					}
					if (RunSize >= RunSizeTmp)//��������� � RUN ������
					{
						RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
						Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
					}
					if (((i->second[SIZE_POINT-1]  >> 5) & 1) == 0)Run[++RunSize - 1] = i;
					i->second[SIZE_POINT-1]  |= 1 << 5;//�������� 6 ���				
				}
				else
				{	//������� ����������
					iTmp = LifePoint.try_emplace(LifePoint.end(), HashPoint({ ull.L[0] + x - 1, ull.L[1] + y - 1 }));//!!
					if (iTmp->second[SIZE_POINT - 1] == 0) iTmp->second[SIZE_POINT - 1] = 144;
						++iTmp->second[SIZE_POINT-1] ;
						if (RunSize >= RunSizeTmp)//��������� � RUN ������
						{
							RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
							Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
						}
						if (((iTmp->second[SIZE_POINT-1]  >> 5) & 1) == 0)Run[++RunSize - 1] = iTmp;
						iTmp->second[SIZE_POINT-1]  |= 1 << 5;//�������� 6 ���
						////////////��������� ����������� ������/////////////
						ic.I[yx] = iTmp;
						++yx;

				}
			}
		}
		if (AreaXmin > ull.L[0])AreaXmin = ull.L[0];//������ ������ 
		if (AreaYmin > ull.L[1])AreaYmin = ull.L[1];
		if (AreaXmax < ull.L[0])AreaXmax = ull.L[0];
		if (AreaYmax < ull.L[1])AreaYmax = ull.L[1];
	}
}

void Calc::RunLife(Grid& grid)
{
	long RunSizeNew = RunNew.size(); //������ ������� RUNtmp
	long RunSizeTmp = 0;
	for (long j = 0; j < RunSize; ++j) //������� ����� ��������� �� RUN
	{
		Run[j]->second[SIZE_POINT - 1] &= 223;//��������� 6� ��� (&11111011) - ��������� ����������
		unsigned char ss = Run[j]->second[SIZE_POINT - 1] & 15; //(&11110000)
		if (ss == 3 && (((Run[j]->second[SIZE_POINT - 1] >> 6) & 1) == 0))//��������� ���� 3 ����� ������
		{
			Run[j]->second[SIZE_POINT - 1] |= 1 << 6; //�������� 7 ��� (�����)
			if (RunSizeTmp >= RunSizeNew)//��������� � RUNnew ������
			{
				RunSizeNew = RunSizeNew + SIZE_ARRAY;
				RunNew.resize(RunSizeNew); //����������� ������ �������, ���� �� �������
			}
			RunNew[++RunSizeTmp-1]= Run[j];
		}
		else if (ss < 1 && (((Run[j]-> second[SIZE_POINT - 1] >> 6) & 1) == 0))//������� ������ ������
		{
			LifePoint.erase(Run[j]);
		}
		else if((ss > 3 || ss < 2) && (((Run[j]->second[SIZE_POINT - 1] >> 6) & 1) == 1))//�������
		{
			Run[j]->second[SIZE_POINT - 1] &= ~(1 << 6);//Run[j]->second.life = false;
			if (RunSizeTmp >= RunSizeNew)//��������� � RUNnew ������
			{
				RunSizeNew = RunSizeNew + SIZE_ARRAY;
				RunNew.resize(RunSizeNew); //����������� ������ �������, ���� �� �������
			}
			RunNew[++RunSizeTmp - 1] = Run[j];
		}
	}

	RunSize = 0;
	for (long j = 0; j < RunSizeTmp; ++j) //������� ����� ��������� �� RUN
	{
		if (((RunNew[j]->second[SIZE_POINT - 1] >> 6) & 1) == 1)
		{
			InsertRun(RunNew[j], false, grid);
		}
		else if (((RunNew[j]->second[SIZE_POINT - 1] >> 6) & 1) == 0)
		{
			InsertRun(RunNew[j], true, grid);
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
	//������� ������
	IC ic;
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	for (i = LifePoint.begin(); i != LifePoint.end(); ++i)
	{
		if (((i->second[SIZE_POINT-1]  >> 6) & 1) == 1)//���� �����
		{
			memcpy(ic.C, i->second, SIZE_POINT);
			delete[] ic.I; //������� ������ 
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
}

Calc::~Calc()
{
}


