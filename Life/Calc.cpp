#include "stdafx.h"
#include "Calc.h"
#include <unordered_map>
#include <map>

static long generation=0;
static long areaXmin= LONG_MAX;
static long areaYmin= LONG_MAX;
static long areaXmax= LONG_MIN;
static long areaYmax= LONG_MIN;
//static std::vector<Point> LifePointRun;
//long LifePointRunSize;

static std::unordered_map<LONGLONG, Point> LifePointRun;


Calc::Calc()
{
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

void Calc::Insert(Point point, std::unordered_map <LONGLONG, Point> &LifePoint, bool pointDelete)
{
	long LifePointRunSizeTmp = LifePointRun.size(); //������ ������� RUN
	int n[3] = { -1, 0, 1 };
	LONGLONG hashPoint;// = HashPoint(point);
	Point pointTmp;
	std::unordered_map<LONGLONG, Point>::iterator i;

	if (pointDelete) //���� ������� �����
	{
		for (int y = 0; y < 3; y++) // �������� �� 8 �������� ������
		{
			for (int x = 0; x < 3; x++)
			{
				pointTmp = { point.x + x - 1,point.y + y - 1 };
				hashPoint = HashPoint(pointTmp);
				i = LifePoint.find(hashPoint);
				if (y == 1 && x == 1)//������� ���� ����
				{ 
					if (i != LifePoint.end()) 
					{
						i->second.life = false;
						LifePointRun[i->first] = i->second; ; //�������������
					}
				}
				else //������� ����������
				{
					if (i != LifePoint.end())
					{
						i->second.state--;
						LifePointRun[i->first] = i->second;
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
				pointTmp = { point.x + x - 1,point.y + y - 1,0,true };
				hashPoint = HashPoint(pointTmp);
				i = LifePoint.find(hashPoint);
					if (i == LifePoint.end())
					{
						LifePoint[HashPoint(pointTmp)] = pointTmp;
						LifePointRun[hashPoint] = pointTmp;
					}
					else
					{
						i->second.life = true;
						LifePointRun[i->first] = i->second;
					}
				}
				else
				{	//������� ����������
					pointTmp = { point.x + x - 1,point.y + y - 1,1,false };
					hashPoint = HashPoint(pointTmp);
					i = LifePoint.find(hashPoint);
					{
						if (i == LifePoint.end())
						{
							LifePoint[HashPoint(pointTmp)] = pointTmp;
							LifePointRun[hashPoint] = pointTmp;
						}
						else
						{
							i->second.state++;
							LifePointRun[i->first] = i->second;
						}
					}
				}
			}
		}
		if (areaXmin > point.x)areaXmin = point.x;//������ ������ 
		if (areaYmin > point.y)areaYmin = point.y;
		if (areaXmax < point.x)areaXmax = point.x;
		if (areaYmax < point.y)areaYmax = point.y;
	}
}

long Calc::Generation(){ return generation;}
long Calc::AreaXmin() { return areaXmin; }
long Calc::AreaYmin() { return areaYmin; }
long Calc::AreaXmax() { return areaXmax; }
long Calc::AreaYmax() { return areaYmax; }

void Calc::RunLife()
{
	std::unordered_map<LONGLONG, Point> LifePointRunOld(LifePointRun);
	//std::unordered_map<LONGLONG, Point> LifePointOld(LifePointRun);
	LifePointRunOld = LifePointRun;
	LifePointRun.clear();
	//LifePointOld = LifePoint;

	std::unordered_map<LONGLONG, Point>::iterator i;
	for (i = LifePointRunOld.begin(); i != LifePointRunOld.end(); i++) //��������� ������� ��������� � RUN
	{
		if (i->second.state == 3 && i->second.life==false)
		{
			Calc::Insert(i->second, LifePoint, false);
		}
		else if((i->second.state > 3 || i->second.state < 2) && i->second.life == true)
		{
			Calc::Insert(i->second, LifePoint, true);
		}
	}

	//������� ������ �����
	std::vector<std::unordered_map<LONGLONG, Point>::iterator> NullPoint(LifePoint.size());
	long NullPointSize = 0;
	for (i = LifePoint.begin(); i != LifePoint.end(); i++) 
	{
		if (i->second.state < 1 && i->second.life==false)
		{
			NullPoint[NullPointSize] = i;
			NullPointSize++;
		}
	}
	for (long j = 0; j < NullPointSize; j++)
	{
		LifePoint.erase(NullPoint[j]);
	}

	++generation;



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
	//LifePointRunSize = 0;
	areaXmin = LONG_MAX;
	areaYmin = LONG_MAX;
	areaXmax = LONG_MIN;
	areaYmax = LONG_MIN;
}

Calc::~Calc()
{
}


