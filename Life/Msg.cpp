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

void Msg::MsgOverflow()
{
		MessageBox(shWnd, L"Overflow Life\n��������� ������ ���������� ����� ������.", L"Life", MB_ICONERROR + MB_OK);
}
Msg::~Msg()
{
}
