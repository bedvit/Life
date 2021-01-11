#include "stdafx.h"
#include <string>
#include <fstream>
#include "rle.h"
#include "Point.h"
#include <algorithm>

Rle::Rle()
{
}

union ULL
{
	LONGLONG U;
	long L[2];
};

void Rle::Save(std::wstring name, Calc& calc)
{
	POINT point;
	long areaXmin = LONG_MAX;
	long areaYmin = LONG_MAX;
	long areaXmax = LONG_MIN;
	long areaYmax = LONG_MIN;
	long dupO = 0;
	long dupB = 0;
	long dup$ = 0;
	long dupOold = 0;
	long dupBold = 0;
	long dup$old = 0;
	long strLineSize = 0;
	ULL ull;

	//////////////////расчет ареала по живым для сохранения в файл
	std::unordered_map<LONGLONG, unsigned char [SIZE_POINT]>::iterator i;
	for (i = calc.LifePoint.begin(); i != calc.LifePoint.end(); ++i)
	{
		if (i->second[SIZE_POINT - 2] == 1)
		{
			ull.U = i->first;
			if (areaXmin > ull.L[0])areaXmin = ull.L[0];//расчет ареала 
			if (areaYmin > ull.L[1])areaYmin = ull.L[1];
			if (areaXmax < ull.L[0])areaXmax = ull.L[0];
			if (areaYmax < ull.L[1])areaYmax = ull.L[1];
		}
	}
	//////////////////


	std::string strOut("x = " + std::to_string(areaXmax - areaXmin+1) + ", y = " + std::to_string(areaYmax - areaYmin+1) + ", rule = B3/S23\n");
	std::string strLine;
	std::string strTmp;

	for (long y = areaYmin; y <= areaYmax; ++y)
	{
		for (long x = areaXmin; x <= areaXmax; ++x)
		{
			if (dup$ > 0 && dupO > 0 && dupO == dupOold)
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

			if (calc.Contains({ x,y }, calc.LifePoint)) ++dupO; else ++dupB;
			if (dup$ > dup$old) dupB = dupB - dupBold;//если новая строка стираем последние пустые

			if (dup$ > 0 && dupO > 0)
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

			if (dupO == dupOold && dupO > 0 && dupB > 0) //считаем количество заполненных/пустых/новых строк
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
	POINT point;
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
				for (long i = 0; i < s.size(); ++i)
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
								if (calc.Insert(point, calc.LifePoint, false, grid)) //Overflow Life
								{
									goto end_;
								}
								++x;
							}
							else
							{
								for (long j = 0; j < repeat; ++j)
								{
									point = { x,y };
									if (calc.Insert(point, calc.LifePoint, false, grid)) //Overflow Life
									{
										goto end_;
									}
									++x;
								}
							}
						}
						else if (s[i] == 'b')
						{
							if (repeat == 0) ++x; else x = x + repeat;
						}
						else if (s[i] == '$')
						{
							if (repeat == 0)
							{
								++y;
								x = 0;
							}
							else
							{
								y += repeat;
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
	double scX = (double)rect.right / ((calc.AreaXmax - calc.AreaXmin + 1));
	double scY = (double)rect.bottom / ((calc.AreaYmax - calc.AreaYmin + 1));
	if (scX > scY) scX = scY;// масштаб по макс стороне шаблона
	if (scX > 32) scX = 32; //макс 32 пикселей
	double scale=33;
	if (scX < 1) scX = (long)(-1.00 / scX-1);
	while (grid.scalePoint != scX)//подгоняем масштаб до степени двойки
	{
		if (scale == 1) scale = -2;
		else if (scale <= -32) scale *= 2;
		else --scale;

		if (scale <= scX)
		{
			grid.scalePoint = scale;
			scX = grid.scalePoint;
		}
	}
	////////////////////////////////
}

Rle::~Rle()
{
}

