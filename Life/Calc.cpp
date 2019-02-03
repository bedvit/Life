#include "stdafx.h"
#include "Calc.h"
#include <unordered_map>
#include <map>

#define SIZE_ARRAY 1000000

static long generation = 0;
//static long population = 0;
static long areaXmin= LONG_MAX;
static long areaYmin= LONG_MAX;
static long areaXmax= LONG_MIN;
static long areaYmax= LONG_MIN;
static std::vector<std::unordered_map<LONGLONG, Point>::iterator> LifePointRun;
static std::vector<std::unordered_map<LONGLONG, Point>::iterator> LifePointRunNew;
long LifePointRunSize;
long LifePointRunSizeTmp;

//static std::unordered_map<LONGLONG, Point> LifePointRun;


Calc::Calc()
{
//Calc:population = population;
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
	//return (i != LifePoint.end()) && i->second.life;
}
void Calc::Insert(Point point, std::unordered_map <LONGLONG, Point> &LifePoint, bool pointDelete) //������� ��� ����� ��������
{
	Point pointTmp = { point.x,point.y};
	const LONGLONG hashPoint = HashPoint(pointTmp);
	std::unordered_map<LONGLONG, Point>::iterator i;
	i = LifePoint.find(hashPoint); //��� ��������� ����� ������ ������ ���� �� �������� ��������
	//typedef std::unordered_map<std::string, int>::iterator iter;
	//const std::pair< iter, bool> result;
	//auto result2;
	if (!pointDelete)
	{
		if (i == LifePoint.end())
		{
			//i = LifePoint.emplace(std::pair<LONGLONG, Point>({ hashPoint ,(Point)pointTmp }));
			i = LifePoint.emplace(hashPoint, pointTmp).first;
		}
	}
	InsertRun(i, pointDelete);
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
						population--;
						i->second.life = false;
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
					population++;
					i->second.life = true;
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
		if (areaXmin > i->second.x)areaXmin = i->second.x;//������ ������ 
		if (areaYmin > i->second.y)areaYmin = i->second.y;
		if (areaXmax < i->second.x)areaXmax = i->second.x;
		if (areaYmax < i->second.y)areaYmax = i->second.y;
	}
}

long Calc::Generation(){ return generation;}
long Calc::AreaXmin() { return areaXmin; }
long Calc::AreaYmin() { return areaYmin; }
long Calc::AreaXmax() { return areaXmax; }
long Calc::AreaYmax() { return areaYmax; }

void Calc::RunLife()
{
	long LifePointRunSizeNew = LifePointRunNew.size(); //������ ������� RUNtmp
	//long LifePointRunSizeOld = LifePointRunSize;
	long LifePointRunSizeTmp = 0;
	for (long j = 0; j < LifePointRunSize; j++) //������� ����� ��������� �� RUN
	{
		LifePointRun[j]->second.update = 0;
		if (LifePointRun[j]->second.state == 3 && LifePointRun[j]->second.life ==false)
		{
			LifePointRun[j]->second.life = true;
			if (LifePointRunSizeTmp >= LifePointRunSizeNew)//��������� � RUNnew ������
			{
				LifePointRunSizeNew = LifePointRunSizeNew + SIZE_ARRAY;
				LifePointRunNew.resize(LifePointRunSizeNew); //����������� ������ �������, ���� �� �������
			}
			LifePointRunNew[++LifePointRunSizeTmp-1]= LifePointRun[j];
		}
		else if((LifePointRun[j]->second.state > 3 || LifePointRun[j]->second.state < 2) && LifePointRun[j]->second.life == true)
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
	
	++generation;


	//std::unordered_map<LONGLONG, Point> LifePointRunOld(LifePointRun);
//std::unordered_map<LONGLONG, Point> LifePointOld(LifePointRun);
//LifePointRunOld = LifePointRun;
//LifePointRun.clear();
//LifePointOld = LifePoint;

//std::unordered_map<LONGLONG, Point>::iterator i;
//for (i = LifePointRunOld.begin(); i != LifePointRunOld.end(); i++) //��������� ������� ��������� � RUN
//{
//	if (i->second.state == 3 && i->second.life==false)
//	{
//		Calc::Insert(i->second, LifePoint, false);
//	}
//	else if((i->second.state > 3 || i->second.state < 2) && i->second.life == true)
//	{
//		Calc::Insert(i->second, LifePoint, true);
//	}
//}

////������� ������ �����
//std::vector<std::unordered_map<LONGLONG, Point>::iterator> NullPoint(LifePoint.size());
//long NullPointSize = 0;
//for (i = LifePoint.begin(); i != LifePoint.end(); i++) 
//{
//	if (i->second.state < 1 && i->second.life==false)
//	{
//		NullPoint[NullPointSize] = i;
//		NullPointSize++;
//	}
//}
//for (long j = 0; j < NullPointSize; j++)
//{
//	LifePoint.erase(NullPoint[j]);
//}


	//if (LifePointRunSize >= LifePointRunSizeTmp)
	//{
	//	LifePointRunSizeTmp = LifePointRunSizeTmp + 10000;
	//	LifePointRun.resize(LifePointRunSizeTmp); //����������� ������ �������, ���� �� �������
	//}
	//LifePointRun[++LifePointRunSize - 1] = i->second;


	//long LifePointRunSizeTmp = LifePointRun.size(); //������ ������� RUN
	//std::unordered_map<LONGLONG, Point>::iterator i;	
	//static std::vector<Point> LifePointRunOld(LifePointRun);
	//long LifePointRunSizeOld = LifePointRunSize;// = LifePointRunSize;
	//LifePointRunSize = 0;
	//std::unordered_map <LONGLONG, Point> LifePointOld(LifePoint);
	//LifePointRunSize = 0;

	//if (generation == 0)
	//{
	//	for (i = LifePoint.begin(); i != LifePoint.end(); i++) //��������� ������� ��������� � RUN
	//	{
	//		LifePointRunSizeOld++;
	//		if (LifePointRunSizeOld >= LifePointRunSizeTmp) LifePointRun.resize(10000);
	//		LifePointRun[LifePointRunSizeOld -1] = i->second;
	//	}

	//}

	//for (long j = 0; j< LifePointRunSizeOld; j++) //������� ����� ��������� �� RUN
	//{

	//	if (LifePointRunOld[j].state == 3 && LifePointRunOld[j].life==false)
	//	{
	//		Calc::Insert(LifePointRunOld[j], LifePoint, false);
	//	}
	//	else if(LifePointRunOld[j].state > 3 || LifePointRunOld[j].state < 2)
	//	{
	//		Calc::Insert(LifePointRunOld[j], LifePoint, true);
	//	}

	//}

	//LifePointRunSize = LifePointRunSizeNew;
	

					//LifePoint.erase(hashPoint);
		//LifePointRun.erase(hashPoint);


		//{
		//	//if (!calc.Contains(i->second, LifePointEnd))
		//	//{
		//		//�������� ������ ��� ������� ��������� 5*5 � ������� ������ ���������
		//		for (int y = 0; y < 5; y++) //��������� 25 ���������
		//		{
		//			for (int x = 0; x < 5; x++)
		//			{
		//				boolTmp[y][x] = calc.Contains({ i->second.x + nn[x], i->second.y + nn[y] }, LifePointOld);
		//			}
		//		}

		//		//��������� 8 ��������� �� ������ �� 8 ���������
		//		for (int y = 0; y < 3; y++) // �������� �� ���������� ������� 5*5
		//		{
		//			for (int x = 0; x < 3; x++)
		//			{
		//				//if (!(y == 1 && x == 1))//�� ������� ���� ����
		//				//{
		//					//tmpPoint2 = { 2 + n[y],  2 + n[x] };//����� ������ �������(����� 2*2 �� ��������� �������)

		//				for (int yy = 0; yy < 3; yy++) // �������� �� ������ ����� ���������� ������� 5*5/8 ��������
		//				{
		//					for (int xx = 0; xx < 3; xx++)
		//					{
		//						if (!(yy == 1 && xx == 1))//�� ������� ���� ����
		//						{
		//							//tmpPoint3 = { (2 + n[x]) + n[xx], (2 + n[y]) + n[yy] };//����� ������ ���������� �������
		//							if (boolTmp[2 + n[y] + n[yy]][2 + n[x] + n[xx]]) L++;
		//							if (L > 3) goto nx_; //��� ������ ������� ����� 4 ����� ������
		//						}
		//					}
		//				}

		//			nx_:
		//				tmpPoint = { i->second.x + n[x], i->second.y + n[y] };
		//				//LifePointEnd[HashPoint(tmpPoint)] = tmpPoint; //�������� ��� ������������ ������
		//				if (L < 2 || L > 3)
		//				{

		//					if (boolTmp[2 + n[y]][2 + n[x]]) //���� ����� ����
		//					{
		//						LifePoint.erase(HashPoint(tmpPoint)); //�������
		//						LifePointRun[HashPoint(tmpPoint)] = tmpPoint; //��������
		//					}
		//				}
		//				else if (L == 3)
		//				{
		//					if (!boolTmp[2 + n[y]][2 + n[x]]) //���� ����� �� ����
		//					{

		//						calc.Insert(tmpPoint, LifePoint, false);
		//					}
		//				}
		//				L = 0;
		//			}
		//		}
		//}
	
}

void Calc::DelLife()
{
	this->LifePoint.clear();
	generation = 0;
	population = 0;
	LifePointRunSize = 0;
	LifePointRunSizeTmp = 0;
	areaXmin = LONG_MAX;
	areaYmin = LONG_MAX;
	areaXmax = LONG_MIN;
	areaYmax = LONG_MIN;
}

Calc::~Calc()
{
}


