#include "stdafx.h"
#include "Calc.h"
#include <vector>
#include <utility>

#define SIZE_ARRAY 1000000
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> Run;
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> Run1; //������ � ������
static std::vector<std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator> Run2; //������ � ���������
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

bool Calc::Contains(POINT point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint) //������ std::unordered_map::contains (C++20) //������ �����
{
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	i = LifePoint.find(HashPoint(point));
	if (i == LifePoint.end()) return false;
	return (i->second[SIZE_POINT - 2] == 1);
}

int Calc::Insert(POINT point, std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]> &LifePoint, bool pointDelete, Grid& grid) //������� ��� ����� ��������
{
	Msg msg;
	if (LifePointSize >= SIZE_LIFEPOINT) //Overflow Life
	{
		msg.ErrOverflow();
		return 1;
	}
	
	LONGLONG hashPoint = HashPoint({ point.x,point.y });
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	i = LifePoint.find(hashPoint); //��� ��������� ����� ������ ������ ���� �� �������� ��������
	if (pointDelete) //���� �������
	{
		if (i != LifePoint.end() && (i->second[SIZE_POINT - 2] == 1))//������� ���� ���� ����� ����� � ��� �����
		{
			InsertRun(i, pointDelete, grid); 
		}
	}
	else //���� ���������
	{
		if (i == LifePoint.end())//���� ����� ����� ��� - �������
		{
			//unsigned char [SIZE_POINT] - ������ 4/8 ���� (��� �32), �.�. 1-4 ����� - ��������� �� ������ ���������� (������), ����� ����� - ��������� �����.
			//�������� ������� ���� - (little-endian)
			//LifePoint.second[SIZE_POINT - 4] - 5� ���� (��������� � 0) - �� ������������
			//LifePoint.second[SIZE_POINT - 3] - 6� ���� - ������� ����������	
			//LifePoint.second[SIZE_POINT - 2] - 7� ���� - �����
			//LifePoint.second[SIZE_POINT - 1] - 8� ���� - ���������� ������ ������ (��������)

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
		else //���� ��� ���� 
		{
			if (i->second[SIZE_POINT - 2] == 0) //���� �� ����� - ������������
			{
				if (InsertRun(i, pointDelete, grid)) return 1;
			}
		}
	}
	return 0;
}

int Calc::InsertRun(std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i, bool pointDelete, Grid& grid) //������ �� ����������
{
	Msg msg;
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
		if (RunSize >= RunSizeTmp)//��������� � RUN ������
		{
			RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
			Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
		}
		if (i->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = i;
		i->second[SIZE_POINT - 3] = 1;//�������� 6 ����

		for (int y = 0; y < 8; ++y) // �������� �� 8 �������� ������
		{
			iTmp = ic.I[y];
			--iTmp->second[SIZE_POINT-1];
			if (RunSize >= RunSizeTmp)//��������� � RUN ������
			{
				RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
				Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
			}
			if (iTmp->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = iTmp;
			iTmp->second[SIZE_POINT - 3] = 1;//�������� 6 ����

		}
		delete[] ic.I; //������� ������ 
		i->second[SIZE_POINT - 2] = 0;//��������� 7� ����
		if (!grid.updateBuffer) //������ ����� ���� ��������� ���������� ������
		{
			grid.DrawPoint(i);
		}
	}
	else //���� ���������
	{
		ic.I = new std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator[8];//��������� ������
		memcpy(i->second,ic.C,  SIZE_POINT);
		++Population;
		i->second[SIZE_POINT - 2] = 1;//�������� 7� ����
		if (!grid.updateBuffer) //������ ����� ���� ��������� ���������� ������
		{
			grid.DrawPoint(i);
		}
		if (RunSize >= RunSizeTmp)//��������� � RUN ������
		{
			RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
			Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
		}
		if (i->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = i;
		i->second[SIZE_POINT - 3] = 1;//�������� 6 ����	

		for (int y = 0; y < 3; ++y) // �������� �� 8 �������� ������
		{
			for (int x = 0; x < 3; ++x)
			{
				if (y == 1 && x == 1)//������� ���� ����
				{
				}
				else
				{	//������� ����������
					if (LifePointSize >= SIZE_LIFEPOINT) //Overflow Life
					{
						msg.ErrOverflow();
						return 1;
					}
					iTmp = LifePoint.try_emplace(LifePoint.end(), HashPoint({ ull.L[0] + x - 1, ull.L[1] + y - 1 }));//!!
					++iTmp->second[SIZE_POINT-1] ;
					if (RunSize >= RunSizeTmp)//��������� � RUN ������
					{
						RunSizeTmp = RunSizeTmp + SIZE_ARRAY;
						Run.resize(RunSizeTmp); //����������� ������ �������, ���� �� �������
					}
					if (iTmp->second[SIZE_POINT - 3] == 0)Run[++RunSize - 1] = iTmp;//�������� ����������

					iTmp->second[SIZE_POINT - 3] = 1;//�������� 6 ����
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
	return 0;
}

int Calc::RunLife(Grid& grid)
{
	long RunSize1 = 0; //������ ������� RUN1
	long RunSize2 = 0; //������ ������� RUN2
	long RunSizeTmp1 = Run1.size(); //������ ������� RUNtmp1
	long RunSizeTmp2 = Run2.size(); //������ ������� RUNtmp2
	for (long j = 0; j < RunSize; ++j) //������� ����� ��������� �� RUN
	{
		Run[j]->second[SIZE_POINT - 3] = 0; //��������� 6� ���� - ��������� ����������
		unsigned char ss = Run[j]->second[SIZE_POINT - 1]; //���������� ������ ������ (��������)
		if (ss == 3 && (Run[j]->second[SIZE_POINT - 2] == 0))//��������� ���� 3 ����� ������
		{
			if (RunSize1 >= RunSizeTmp1)//��������� � RUN ������
			{
				RunSizeTmp1 = RunSizeTmp1 + SIZE_ARRAY;
				Run1.resize(RunSizeTmp1); //����������� ������ �������, ���� �� �������
			}
			Run1[++RunSize1-1]= Run[j];
		}
		else if (ss < 1 && (Run[j]-> second[SIZE_POINT - 2] == 0))//������� ������ ������
		{
			LifePoint.erase(Run[j]);
			--LifePointSize;
		}
		else if((ss > 3 || ss < 2) && (Run[j]->second[SIZE_POINT - 2] == 1))//�������
		{
			if (RunSize2 >= RunSizeTmp2)//��������� � RUN ������
			{
				RunSizeTmp2 = RunSizeTmp2 + SIZE_ARRAY;
				Run2.resize(RunSizeTmp2); //����������� ������ �������, ���� �� �������
			}
			Run2[++RunSize2 - 1] = Run[j];
		}
	}

	RunSize = 0; //�������� ������ RUN - ���������� ������
	for (long j = 0; j < RunSize1; ++j) //�����
	{
		if (InsertRun(Run1[j], false, grid)) return 1;
	}

	for (long j = 0; j < RunSize2; ++j) //���������
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
	//������� ������
	IC ic;
	long x=0;
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	for (i = LifePoint.begin(); i != LifePoint.end(); ++i)
	{
		x++;
		if (i->second[SIZE_POINT - 2] == 1)//���� �����
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
	LifePointSize=0;
}

Calc::~Calc()
{
}


