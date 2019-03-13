#pragma once
#include <iostream>
#include "Calc.h"
#include "Grid.h"
class Rle
{
public:
	Rle();
	~Rle();

	void Save(std::wstring name, Calc& calc);
	void Load(std::wstring name, Calc&calc, RECT rect, Grid& grid);
};

