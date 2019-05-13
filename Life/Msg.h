#pragma once
class Msg
{
public:
	Msg();
	~Msg();

	void SetHWND(HWND hWnd);
	void ErrOverflow();
	void OutOfRange();
};

