#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "rle.h"
#include "Calc.h"
#include "Point.h"
#include <algorithm>


Rle::Rle()
{
}

void Rle::Save(std::wstring name, Calc& calc)
{
	Point point;
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
	long strLineSize = 0;

	std::string strOut("x = " + std::to_string(areaXmax - areaXmin+1) + ", y = " + std::to_string(areaYmax - areaYmin+1) + ", rule = B3/S23\n");
	std::string strLine;
	std::string strTmp;

	for (long y = areaYmin; y <= areaYmax; y++)
	{
		for (long x = areaXmin; x <= areaXmax; x++)
		{
			point = { x,y };
			if (calc.Contains(point, calc.LifePoint)) dupO++; else dupB++;


			if (dup$ > dup$old) dupB = dupB - dupBold;//если новая строка стираем последние пустые

			if (dup$ == dup$old && dup$ > 0 && dupO > 0)
			{
				if (dup$ == 1) strTmp = "$"; else	strTmp = std::to_string(dup$) + "$";
				dup$ = 0;

				if (strLine.length() + strTmp.length() > 70) //если длина строки превышает 70 символов
				{
					strOut = strOut + strLine + "\n";
					strLine = strTmp;
				}
				else
				{
					strLine = strLine + strTmp;
				}
			}

			if (dupB == dupBold && dupB > 0 && dupO > 0)
			{
				if (dupB == 1) strTmp = "b"; else	strTmp = std::to_string(dupB) + "b";
				dupB = 0;

				if (strLine.length() + strTmp.length() > 70) //если длина строки превышает 70 символов
				{
					strOut = strOut + strLine + "\n";
					strLine = strTmp;
				}
				else
				{
					strLine = strLine + strTmp;
				}
			}

			if (dupO == dupOold && dupO > 0) //считаем количество заполненных/пустых/новых строк
			{
				if (dupO == 1) strTmp = "o"; else	strTmp = std::to_string(dupO) + "o";
				dupO = 0;

				if (strLine.length() + strTmp.length() > 70) //если длина строки превышает 70 символов
				{
					strOut = strOut + strLine + "\n";
					strLine = strTmp;
				}
				else
				{
					strLine = strLine + strTmp;
				}
			}
			dupOold = dupO;
			dupBold = dupB;
			dup$old = dup$;
		}
		dup$++;
	}

	if (dupO == dupOold && dupO != 0) //считаем количество заполненных/пустых/новых строк
	{
		if (dupO == 1) strTmp = "o"; else	strTmp = std::to_string(dupO) + "o";
		dupO = 0;

		if (strLine.length() + strTmp.length() > 70) //если длина строки превышает 70 символов
		{
			strOut = strOut + strLine + "\n";
			strLine = strTmp;
		}
		else
		{
			strLine = strLine + strTmp;
		}
	}

	//strOut = strOut + strLine + "!";


	if (strLine.length() > 69) //если длина строки превышает 69 символов
	{
		strOut = strOut + strLine + "\n";
		strOut = strOut + "!";
	}
	else
	{
		strOut = strOut + strLine + "!";
	}

	std::ofstream file(name); // поток для записи
	//out.open(name); // окрываем файл для записи
	if (file.is_open()) file << strOut;
	file.close();
	return;
}

void Rle::Load(std::wstring name, Calc& calc, RECT rect, Grid& grid)
{
	std::ifstream file(name);
	std::string s;
	long count=0;
	long countTmp=0;
	long repeat = 0;
	Point point;
	long x = 0;
	long y = 0;

	if (file.is_open())
	{
		while (getline(file, s))
		{
			s.erase(std::remove(s.begin(), s.end(), ' '), s.end()); //убираем пробелы
			if (s[0] == '0' || s[0] == '1' || s[0] == '2' || s[0] == '3' || s[0] == '4' || s[0] == '5' || s[0] == '6' || s[0] == '7' || s[0] == '8' || s[0] == '9' || s[0] == 'o' || s[0] == 'b' || s[0] == '$' || s[0] == '!' || s[0] == ' ' || s[0] == 'о') //пропускаем строки только с такими начальными символами
			{
				countTmp = -1;
				for (long i = 0; i < s.size(); i++)
				{
					if (s[i] == '!') goto end_;
					if (s[i] == 'o' || s[i] == 'b' || s[i] == '$' || s[i] == 'о') //если найден символ, считаем
					{
						count = i - countTmp - 1;
						repeat = std::stoi('0' + s.substr(countTmp + 1, count)); //(позиция, длина)//количество повторов символа
						countTmp = i;
						if (s[i] == 'o' || s[i] == 'о')
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

	//автомасштабирование
	long scX= rect.right / ((calc.AreaXmax() - calc.AreaXmin() + 1));
	long scY = rect.bottom / ((calc.AreaYmax() - calc.AreaYmin() + 1));
	if (scX > scY) scX = scY;//; else grid.scale = sc1;
	if (scX > 50) scX = 50; //макс 50 пикселей
	if (scX <= 0) scX = 1; //мин 1 пикселей
	grid.scale = scX;
}

Rle::~Rle()
{
}

