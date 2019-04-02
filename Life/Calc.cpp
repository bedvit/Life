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

bool Calc::Contains(Point point, std::unordered_map<LONGLONG, unsigned char> &LifePoint) //������ std::unordered_map::contains (C++20) //������ �����
{
	std::unordered_map<LONGLONG, unsigned char>::iterator i;
	i = LifePoint.find(HashPoint(point));
	if (i == LifePoint.end()) return false;
	return ((bool)((i->second >> 6) & 1));
}
void Calc::Insert(Point point, std::unordered_map<LONGLONG, unsigned char> &LifePoint, bool pointDelete, Grid& grid) //������� ��� ����� ��������
{
	LONGLONG hashPoint = HashPoint({ point.x,point.y });
	std::unordered_map<LONGLONG, unsigned char>::iterator i;
	i = LifePoint.find(hashPoint); //��� ��������� ����� ������ ������ ���� �� �������� ��������
	if (pointDelete) //���� �������
	{
		if (i != LifePoint.end() && ((bool)((i->second >> 6) & 1))) InsertRun(i, pointDelete, grid); //������� ���� ���� ����� ����� � ��� �����
	}
	else //���� ���������
	{
		if (i == LifePoint.end())//���� ����� ����� ��� - �������
		{
			i = LifePoint.emplace(hashPoint, 144).first;
			InsertRun(i, pointDelete, grid);
		}
		else //���� ��� ���� �����
		{
			if (!(bool)((i->second >> 6) & 1)) InsertRun(i, pointDelete, grid); //������� ���� �� ���� �����//3� ���� � ������, 6� � ����� - (little-endian)
		}
	}
}

void Calc::InsertRun(std::unordered_map<LONGLONG, unsigned char>::iterator i, bool pointDelete, Grid& grid) //������ �� ����������
{
	long LifePointRunSizeTmp = LifePointRun.size(); //������ ������� RUN
	int n[3] = { -1, 0, 1 };
	LONGLONG hashPointTmp;
	std::unordered_map<LONGLONG, unsigned char>::iterator iTmp;
	ULL ull;
	ull.U = i->first;

	if (pointDelete) //���� ������� �����
	{
		for (int y = 0; y < 3; ++y) // �������� �� 8 �������� ������
		{
			for (int x = 0; x < 3; ++x)
			{
				if (y == 1 && x == 1)//������� ���� ����
				{ 
						--Population;
						i->second &= ~(1 << 6);//��������� 7� ���
						if (!grid.updateBuffer) //������ ����� ���� ��������� ���������� ������
						{
							grid.DrawPoint(i);
						}
						
						if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
						}
						if (!(bool)((i->second >> 5) & 1))LifePointRun[++LifePointRunSize - 1] = i;
						i->second |= 1 << 5; //�������� 6 ���
				}
				else //������� ����������
				{
					iTmp = LifePoint.find(HashPoint({ ull.L[0] + x - 1, ull.L[1] + y - 1 }));
						--iTmp->second;
						if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
						{
							LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
							LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
						}
						if (!(bool)((iTmp->second >> 5) & 1))LifePointRun[++LifePointRunSize - 1] = iTmp;
						iTmp->second |= 1 << 5; //�������� 6 ���
				}
			}
		}
	}
	else //���� ���������
	{
		for (int y = 0; y < 3; ++y) // �������� �� 8 �������� ������
		{
			for (int x = 0; x < 3; ++x)
			{
				if (y == 1 && x == 1)//������� ���� ����
				{
					++Population;
					i->second |= 1 << 6; //�������� 7� ���
					if (!grid.updateBuffer) //������ ����� ���� ��������� ���������� ������
					{
						grid.DrawPoint(i);
					}
					if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
					{
						LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
						LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
					}
					if (((i->second >> 5) & 1) == 0)LifePointRun[++LifePointRunSize - 1] = i;
					i->second |= 1 << 5;//�������� 6 ���				
				}
				else
				{	//������� ����������
					iTmp = LifePoint.try_emplace(HashPoint({ ull.L[0] + x - 1, ull.L[1] + y - 1 }), 144).first;
					++iTmp->second;
					if (LifePointRunSize >= LifePointRunSizeTmp)//��������� � RUN ������
					{
						LifePointRunSizeTmp = LifePointRunSizeTmp + SIZE_ARRAY;
						LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
					}
					if (!(bool)((iTmp->second >> 5) & 1))LifePointRun[++LifePointRunSize - 1] = iTmp;
					iTmp->second |= 1 << 5;//�������� 6 ���
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
	long LifePointRunSizeNew = LifePointRunNew.size(); //������ ������� RUNtmp
	long LifePointRunSizeTmp = 0;
	for (long j = 0; j < LifePointRunSize; ++j) //������� ����� ��������� �� RUN
	{
		LifePointRun[j]->second &= 223;//��������� 6� ��� (&11111011) - ��������� ����������
		unsigned char ss = LifePointRun[j]->second & 15; //(&11110000)
		if (ss == 3 && (!(bool)((LifePointRun[j]->second >> 6) & 1)))//��������� ���� 3 ����� ������
		{
			LifePointRun[j]->second |= 1 << 6; //�������� 7 ��� (�����)
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//��������� � RUNnew ������
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //����������� ������ �������, ���� �� �������
			}
			LifePointRunNew[++LifePointRunSizeTmp-1]= LifePointRun[j];
		}
		else if (ss < 1 && (!(bool)((LifePointRun[j]->second >> 6) & 1)))//������� ������ ������
		{
			LifePoint.erase(LifePointRun[j]);
		}
		else if((ss > 3 || ss < 2) && ((bool)((LifePointRun[j]->second >> 6) & 1)))//�������
		{
			LifePointRun[j]->second &= ~(1 << 6);//LifePointRun[j]->second.life = false;
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//��������� � RUNnew ������
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //����������� ������ �������, ���� �� �������
			}
			LifePointRunNew[++LifePointRunSizeTmp - 1] = LifePointRun[j];
		}
	}

	LifePointRunSize = 0;
	for (long j = 0; j < LifePointRunSizeTmp; ++j) //������� ����� ��������� �� RUN
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


