#pragma once
#include <iostream>
#include "Calc.h"
class Rle
{
public:
	Rle();
	~Rle();
	void Save(std::wstring name, Calc& calc);
	void Load(std::wstring name, Calc&calc);
};

