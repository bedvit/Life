#include "stdafx.h"
#include "Msg.h"

static HWND shWnd;

Msg::Msg()
{
}

void Msg::SetHWND(HWND hWnd)
{
	shWnd = hWnd;
}

void Msg::ErrOverflow()
{
		MessageBox(shWnd, L"Life Overflow\n��������� ������ ���������� ����� ������!", L"Life", MB_ICONERROR + MB_OK);
}

void Msg::OutOfRange()
{
	MessageBox(shWnd, L"Out of range\n����� �� ��������� �������� ����", L"Life", MB_ICONEXCLAMATION + MB_OK);
}
Msg::~Msg()
{
}
