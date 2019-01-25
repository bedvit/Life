#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "rle.h"
#include "Calc.h"
#include <algorithm>




Rle::Rle()
{
}

void Rle::Save(std::wstring name, Calc& calc)
{
	POINT point;
	long areaXmin = calc.AreaXmin();
	long areaYmin = calc.AreaYmin();
	long areaXmax = calc.AreaXmax();
	long areaYmax = calc.AreaYmax();
	long dupO = 0;
	long dupB = 0;
	long dup$ = 0;
	long dupOold = 0;
	long dupBold = 0;
	long dup$old = 0;
	//long dupChar = 0;
	//long dupCharOld = 0;

	std::string strOut("x = " + std::to_string(areaXmax - areaXmin+1) + ", y = " + std::to_string(areaYmax - areaYmin+1) + ", rule = B3 / S23\n");
	std::string strTmp;

	for (long y = areaYmin; y <= areaYmax; y++)
	{
		for (long x = areaXmin; x <= areaXmax; x++)
		{
			point = { x,y };
			if (calc.Contains(point, calc.LifePoint))
			{
				dupO++;
				strTmp += "o";
			}
			else
			{
				dupB++;
				strTmp += "b";
			}
			if (dupO == dupOold)
			{
				strTmp = strTmp.to_string(dupO) + "o";
			}

			long dupOold = dupO;
			long dupBold = dupB;
			long dup$old = dup$;

		}
		dup$++;
		strTmp += "$";

	}
	strTmp += "!";

	//std::string strOutEnd;
	//char ch ='x';
	//char chTmp ='x';
	//for (long i = 0; i <= strOut.size; i++)
	//{
	//	chTmp=
	//}

	strOut = strOut + strTmp;
	std::ofstream file(name); // ����� ��� ������
	//out.open(name); // �������� ���� ��� ������
	if (file.is_open()) file << strOut;
	file.close();
	return;
}

void Rle::Load(std::wstring name, Calc& calc, RECT rect, Grid& grid)
{
	std::ifstream file(name);
	std::string s;
	//std::string sTmp;
	long count=0;
	long countTmp=0;
	long repeat = 0;
	POINT point;
	long x = 0;
	long y = 0;

	if (file.is_open())
	{
		while (getline(file, s))
		{
			s.erase(std::remove(s.begin(), s.end(), ' '), s.end()); //������� �������
			if (s[0] == '0' || s[0] == '1' || s[0] == '2' || s[0] == '3' || s[0] == '4' || s[0] == '5' || s[0] == '6' || s[0] == '7' || s[0] == '8' || s[0] == '9' || s[0] == 'o' || s[0] == 'b' || s[0] == '$' || s[0] == '!' || s[0] == ' ' || s[0] == '�') //���������� ������ ������ � ������ ���������� ���������
			{
				countTmp = -1;
				for (long i = 0; i < s.size(); i++)
				{
					if (s[i] == '!') goto end_;
					if (s[i] == 'o' || s[i] == 'b' || s[i] == '$' || s[i] == '�') //���� ������ ������, �������
					{
						count = i - countTmp - 1;
						//sTmp = '0' + s.substr(countTmp + 1, count);
						repeat = std::stoi('0' + s.substr(countTmp + 1, count)); //(�������, �����)//���������� �������� �������
						countTmp = i;
						if (s[i] == 'o' || s[i] == '�')
						{
							if (repeat == 0)
							{
								point = {x,y};
								calc.Insert(point, calc.LifePoint, false);
								x++;
							}
							else
							{
								for (long j = 0; j < repeat; j++)
								{
									point = { x,y };
									calc.Insert(point, calc.LifePoint, false);
									x++;
								}
							}
						}
						else if (s[i] == 'b')
						{
							if (repeat == 0) x++; else x = x + repeat;
						}
						else if (s[i] == '$')
						{
							if (repeat == 0)
							{
								y++;
								x = 0;
							}
							else
							{
								y = y + repeat;
								x = 0;
							}
						}

					}
				}
			}
		}
	}
	end_:
	file.close();

	//�������������������
	long scX= rect.right / ((calc.AreaXmax() - calc.AreaXmin() + 1));
	long scY = rect.bottom / ((calc.AreaYmax() - calc.AreaYmin() + 1));
	if (scX > scY) scX = scY;//; else grid.scale = sc1;
	if (scX > 50) scX = 50; //���� 50 ��������
	if (scX <= 0) scX = 1; //��� 1 ��������
	grid.scale = scX;
}

Rle::~Rle()
{
}

