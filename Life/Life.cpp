﻿// Life.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"

//#include "Grid.h"

#include "Life.h"
//#include "Point.h"
#include "rle.h"
#include <thread>
#include <Windows.h>
#include "commdlg.h"
#include "shellapi.h"
#include "Calc.h"

#define MAX_LOADSTRING 100
#define WM_SETFONT     0x0020

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// $$$$$ Это глобальная переменная класса Grid, который я написал. 
// Этот класс поможет нам а) рисовать сетку б) красить в ней ячейки.
// Он позволит нам думать что у нас есть бесконечное поле точек. 

Grid grid;
Calc calc;
Rle rle;
Point point;

// $$$$$ Класс point написал тоже я. Это просто пара целых переменных x, y. Им можно задавать координаты ячейки грида, координаты пикселя, разность между двумя точками. Полезный короче. 
// В стандартной библиотеке есть такой класс, но я хотел чтобы ты заценил насколько он простой. Потом я его убью и буду юзать типовой.
Point size; // $$$$$ Сюда будем присваивать размеры вьюпорта в пикселях и передавать для рисования в Grid.
Point mousePos; // $$$$$ Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение
Point mousePosPoint; //Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение, ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
Point calcPoint;

//bool RunCalc=false; //запустить расчет жизни 
//bool CalcEnd=false; //вычисления закончены - готов расчет нового поколения
//bool UpdateLine = true; //отрисовать сетку если нужно еще раз
static int wheelDelta = 0; // $$$$$ требуется для считывания колесика мышки
bool DragEnabled; // $$$$$ Для таскания грида правой кнопкой мыши
bool LbuttonClick; // $$$$$ Для выделения ячеек левой кнопкой мыши
bool pointDelete; // $$$$$ Для выделения/снятия выделения ячеек левой кнопкой мыши, при клике и движении мышки
unsigned int start_time; // = clock();
unsigned int end_time; // = clock(); // конечное время
unsigned int search_time; // = end_time - start_time; // искомое время
unsigned int pre_time; //
unsigned int generation_time; //
unsigned int start_timeNew;
unsigned int search_timeNew;
unsigned int GenerationFix;
long step;

HWND hWndEdit;
HWND hWndEditS;
//открыть файл
wchar_t* buf = new wchar_t[255];
wchar_t *end;
wchar_t szFileName[MAX_PATH] = L"";
OPENFILENAME ofn;
wchar_t fileOut[MAX_PATH] = L"Life.rle";
wchar_t buffer[255]; //результат для инфо панели
wchar_t bufferTmp[255]; //результат для инфо панели
char vOutChar[255];
HFONT hFont = CreateFont(16, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
//HDC hdcLine;



// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LIFE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LIFE));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIFE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LIFE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd) return FALSE;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   //создаем свое меню
   RECT rect = { 0 };
  // GetWindowRect(hWnd, &rect); //в координатах экрана
	GetClientRect(hWnd, &rect); //в координатах пользовательского окна
	hWndEdit = CreateWindowEx(WS_EX_LEFT, L"Edit", L"0000", WS_CHILD | WS_VISIBLE, rect.right - 100, 230, 100, 14, hWnd, (HMENU)NULL, hInstance, NULL);
	hWndEditS = CreateWindowEx(WS_EX_LEFT, L"Edit", L"1", WS_CHILD | WS_VISIBLE, rect.right - 100, 230, 100, 14, hWnd, (HMENU)NULL, hInstance, NULL);
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	
	// $$$$ координаты мыши на Экране
	long xPos = LOWORD(lParam);
	long yPos = HIWORD(lParam);

	// $$$$$ Получим координаты мыши в рамках клиентского окна 
	// $$$$$ Клиентское окно или вьюпорт это рабочая область формы (без рамки, заголовка и меню).

	POINT p;
	p.x = xPos;
	p.y = yPos;
	ScreenToClient(hWnd, &p); // $$$$$ Спец функция для этого 

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				DestroyWindow(hWndEditS);
				DestroyWindow(hWndEdit);
                DestroyWindow(hWnd);
                break;
			case IDM_START:
					GetWindowTextW(hWndEdit, buf, 255); //забираем данные о замедлении из пользовательского меню
					SetTimer(hWnd, 123, wcstol(buf, &end, 10), NULL);
					start_time = clock()- search_time;
					start_timeNew = clock();
					GenerationFix = calc.Generation;
					GetWindowTextW(hWndEditS, buf, 255); //забираем данные о количестве поколений на один шаг
					step = wcstol(buf, &end, 10);
				break;
			case IDM_STOP:
				KillTimer(hWnd, 123);
				break;
			case IDM_NEW:
				KillTimer(hWnd, 123);
				calc.DelLife();
				search_time = 0;
				GenerationFix = 0;
				grid.position = {0,0}; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях
				grid.scalePoint = 16;
				grid.updateBuffer = true; //перерисовываем сетку
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;
			case IDM_OPEN:
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = L"Text Files (*.rle)\0*.rle\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = L"rle";
				if (GetOpenFileName(&ofn))
				{
					RECT rect;
					GetClientRect(hWnd, &rect);// $$$$$ Узнаем размеры клиентского окна.
					KillTimer(hWnd, 123);
					calc.DelLife();
					search_time = 0;
					GenerationFix = 0;
					grid.position = { 0,0 }; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях
					//ShowWindow(hWnd, SW_MAXIMIZE);//Максимизирует указанное окно.
					rle.Load(ofn.lpstrFile, calc, rect, grid);
				}
				grid.updateBuffer = true; //перерисовываем сетку
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;
			case IDM_SAVE:
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize =  OPENFILENAME_SIZE_VERSION_400; // SEE NOTE BELOW
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = L"Text Files (*.rle)\0*.rle\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFile = fileOut;
				ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
				ofn.lpstrDefExt = L"rle";
				
				if (GetSaveFileName(&ofn))
				{
					rle.Save(ofn.lpstrFile, calc);
				}
				break;
			case IDM_RETURN_TO_ZERO:
				grid.position = { 0,0 };
				grid.updateBuffer = true; //перерисовываем сетку
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;
            default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_TIMER:
		{
				calc.RunLifeStep(step, grid); //запуск расчета поколений

				end_time = clock(); // конечное время
				search_time = end_time - start_time; // искомое время
				search_timeNew = end_time - start_timeNew;
				generation_time = end_time - pre_time; //
				pre_time = end_time; // конечное время

				InvalidateRect(hWnd, NULL, false);//перерисовать клиентское окно еcли готово новое поколение жизни.		

		}
		break;
	case WM_PAINT: // $$$ Событие прорисовки. Вызывается системой когда окно нужно перерисовать. Например если мы его растянули и т.д. Мы сами можешь попросить систему вызвать это событие.(см. ниже) 
		{
			// $$$$$ Узнаем размеры клиентского окна.
			RECT rect;
			GetClientRect(hWnd, &rect);

			size.x = rect.right - rect.left+1;// + 1; //ширина
			size.y = rect.bottom - rect.top+1;// + 1; //высота
			
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);// hdc это инструмент для рисования в клиентской области окна
			HDC hMemDC = CreateCompatibleDC(hdc); //двойная буферизация

			unsigned char* lpBitmapBits;
			BITMAPINFO bi;
			ZeroMemory(&bi, sizeof(BITMAPINFO));
			bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bi.bmiHeader.biWidth = size.x;
			bi.bmiHeader.biHeight = -size.y; //если отрицательное - слева вверху, положительное слева снизу
			bi.bmiHeader.biPlanes = 1;
			bi.bmiHeader.biBitCount = 32;
			bi.bmiHeader.biCompression = BI_RGB;
			HBITMAP bitmap = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
			HGDIOBJ oldBmp = SelectObject(hMemDC, bitmap);

			if (grid.updateBuffer)
			{
				PatBlt(hMemDC, 0, 0, size.x, size.y, WHITENESS);//закрашиваем прямоугольник белым фоном
				//	hdcLine = CreateCompatibleDC(hdc);
				//	HBITMAP hScreen = CreateCompatibleBitmap(hdcLine, size.x, size.y);
				//	HBITMAP oldBmp = (HBITMAP)SelectObject(hMemDC, hScreen);
				//	PatBlt(hdcLine, 0, 0, size.x, size.y, WHITENESS);//закрашиваем прямоугольник белым фоном
				//	//grid.DrawLine(hdcLine, size);
				//	UpdateLine = false;
			}

			//BitBlt(hMemDC, 0, 0, size.x, size.y, hdcLine, 0, 0, SRCCOPY);
			//PatBlt(hMemDC, 0, 0, size.x, size.y, WHITENESS);//закрашиваем прямоугольник белым фоном
			grid.Draw(lpBitmapBits, calc.LifePoint, rect);
			//grid.DrawLine(hMemDC, size);// $$$$$ Скажем гриду, чтобы нарисовал себя в рамках размеров клиентского окна

			//ИНФО ПАНЕЛЬ
			RECT rectTxt; //координаты текста
			SelectObject(hMemDC, hFont);
			SetTextColor(hMemDC, RGB(0, 0, 0));
			long Xstart= rect.right - 100; //для авто-выравнивания
			long Ystart=0;	
			int decimal;
			int sign;

			Ystart += 10;//Масштаб
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Масштаб"), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			if (grid.scalePoint < 1)
			{
				_itow_s(-grid.scalePoint, buffer, 255, 10);
			}
			else
			{
				_itow_s(grid.scalePoint, buffer, 255, 10);
			}
			int iEnd = wcslen(buffer)+1; //колво символов с '\0'
			if (grid.scalePoint<1)
			{
				for (int i=0; i < iEnd-1; i++)
				{
					bufferTmp[i] = buffer[i];
				}
				bufferTmp[iEnd-1]= L':';
				bufferTmp[iEnd] = L'1';
				bufferTmp[iEnd+1] = L'\0';
			}
			else
			{
				bufferTmp[0] = L'1';
				bufferTmp[1] = L':';
				for (int i = 2; i < iEnd+2; i++)
				{
					bufferTmp[i] = buffer[i - 2];
				}
			}
			DrawText(hMemDC, bufferTmp, -1, &rectTxt, DT_NOCLIP);


			Ystart += 30; //Координаты
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Координаты"), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20; 
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("X:"), -1, &rectTxt, DT_NOCLIP);
			SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
			Point calcPoint = grid.GetCell(mousePos);
			_itow_s(calcPoint.x, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Y:"), -1, &rectTxt, DT_NOCLIP);
			SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
			_itow_s(calcPoint.y, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);


			Ystart += 30;//Ареал
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Ареал"), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("X:"), -1, &rectTxt, DT_NOCLIP);
			SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
			_i64tow_s(calc.AreaXmin, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
			Ystart += 15;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Y:"), -1, &rectTxt, DT_NOCLIP);
			SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
			_itow_s(calc.AreaYmin, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("X:"), -1, &rectTxt, DT_NOCLIP);
			SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
			_itow_s(calc.AreaXmax, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
			Ystart += 15;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Y:"), -1, &rectTxt, DT_NOCLIP);
			SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
			_itow_s(calc.AreaYmax, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
			

			Ystart += 30;//Население
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Население"), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			_itow_s(calc.Population, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP); 
			
			
			Ystart += 30;//Поколение
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Поколение"), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			_itow_s(calc.Generation, buffer, 255, 10);
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
			

			Ystart += 30;//Таймер
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Таймер, сек."), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			_gcvt_s(vOutChar, sizeof(vOutChar), ((double)search_time / 1000), 5);
			//_fcvt_s(buffer, _CVTBUFSIZE,search_time/1000, 5, &decimal, &sign); //для char
			mbstowcs_s(NULL, buffer, sizeof(buffer) / 2, vOutChar, sizeof(vOutChar));
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);

			Ystart += 30;//Таймер
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Поколений/сек."), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			double out = 0.0;
			if(search_timeNew != 0) out=((double)(calc.Generation- GenerationFix) * 1000 / search_timeNew);// накопительным итогом на каждый запуск
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			_gcvt_s(vOutChar, sizeof(vOutChar), out, 5);
			mbstowcs_s(NULL, buffer, sizeof(buffer) / 2, vOutChar, sizeof(vOutChar));
			DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);


			Ystart += 30;//Замедление
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Замедление, мс."), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			MoveWindow(hWndEdit, Xstart, Ystart, 100, 14, TRUE);


			Ystart += 30;//Замедление
			SetRect(&rectTxt, Xstart, Ystart, 0, 0);
			DrawText(hMemDC, TEXT("Шаг поколений"), -1, &rectTxt, DT_NOCLIP);
			Ystart += 20;
			MoveWindow(hWndEditS, Xstart, Ystart, 100, 14, TRUE);
			//ИНФО ПЕНЕЛЬ
			

			BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, 0, 0, SRCCOPY);
			SelectObject(hMemDC, oldBmp);

			DeleteDC(hMemDC);
			//DeleteDC(hdcLine);
			DeleteObject(bitmap);
			DeleteObject(oldBmp);
			DeleteDC(hdc);
			EndPaint(hWnd, &ps);

        }
        break;

    case WM_DESTROY: //подчищаем память
		PostQuitMessage(0);
		delete[] buf;
		//delete[] szFileName;
		//delete[] fileOut;
		//delete[] buffer; //результат для инфо панели
		//delete[] bufferTmp; //результат для инфо панели
		//delete[] vOutChar;
        break;

	case WM_LBUTTONDOWN: // $ Левая кнопка нажата
		if (grid.scalePoint >= 1) // не рисуем при масштабе меньше 1 
		{
			LbuttonClick = true; // $$$$$ Включим режим выделения клеток
			mousePos.x = xPos;  // $$$$$ Запомним координаты мыши
			mousePos.y = yPos;
			calcPoint = grid.GetCell(mousePos); // ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
			if (calc.Contains(calcPoint, calc.LifePoint)) pointDelete = true; else pointDelete = false; //смотрим есть ли такой элемент
			mousePosPoint.x = calcPoint.x;
			mousePosPoint.y = calcPoint.y;
			calc.Insert(calcPoint, calc.LifePoint, pointDelete,grid);
			InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
		}
		break;

	case	WM_RBUTTONDOWN: // $ Правая кнопка нажата
		DragEnabled = true; // $$$$$ Включим режим таскания сетки
		mousePos.x = xPos;  // $$$$$ Запомним координаты мыши
		mousePos.y = yPos;

		break;

	case WM_MOUSEMOVE:  // $$$$$ Мышка двигается 
		
		// используем TrackMouseEvent, при выходе за пределы пользовательского окна
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = hWnd;
		tme.dwFlags = TME_LEAVE;
		TrackMouseEvent(&tme);

		if (DragEnabled) // $$$$$ При этом зажата правая кнопка т.к. включен режим таскания
		{
			// $$$$$ двигаем грид на дельту с прошлого события
			grid.Move(xPos - mousePos.x, yPos - mousePos.y);
			mousePos.x = xPos;
			mousePos.y = yPos;
			grid.updateBuffer = true; //перерисовываем сетку
			InvalidateRect(hWnd, NULL, false);
		}
		else if (LbuttonClick)  //включе режим выделения
		{
			//Pause = true;
			if (grid.scalePoint >= 1) // не рисуем при масштабе меньше 1 
			{
				long xPosTmp = xPos - mousePos.x;//разница от текущих координат и записанных в координатах экрана
				long yPosTmp = yPos - mousePos.y;
				long MaxPosTmp;// пропуски мышки, которые мы будем обсчитывать

				if (abs(xPosTmp) > abs(yPosTmp)) MaxPosTmp = abs(xPosTmp); else MaxPosTmp = abs(yPosTmp);//макс пропуск в пикселях
				long ColPosTmp = (MaxPosTmp / grid.scalePoint) + 1;//количество циклов расчета координат по этому пропуску с учетом масштаба

				Point start = mousePos;

				for (long i = 0; i < ColPosTmp; i++)
				{
					mousePos.x = start.x + (float)xPosTmp / ColPosTmp * (i + 1);
					mousePos.y = start.y + (float)yPosTmp / ColPosTmp * (i + 1);

					Point calcPoint = grid.GetCell(mousePos); // ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
					if (mousePosPoint.x != calcPoint.x || mousePosPoint.y != calcPoint.y)
					{
						mousePosPoint.x = calcPoint.x;
						mousePosPoint.y = calcPoint.y;
						calc.Insert(calcPoint, calc.LifePoint, pointDelete, grid);
					}
				}
				mousePos.x = xPos;
				mousePos.y = yPos;
				InvalidateRect(hWnd, NULL, false);//перерисовать клиентское окно
			}

		}

		break;
	case WM_MOUSELEAVE: //Сообщение WM_MOUSELEAVE посылается в окно тогда, когда курсор оставляет рабочую область окна, заданную при предшествующем вызове функции TrackMouseEvent.
		LbuttonClick=false; //отключаем выделение
		break;

	case WM_RBUTTONUP: // $$$$$ Правая кнопка отжата
		DragEnabled = false; // $$$$$ Выключим режим таскания. Если не выключить, сетка будет вечно ходить за мышкой.
		start_timeNew = clock(); //пересчитываем скорость поколений из-за задержки
		GenerationFix = calc.Generation;
		break;
	case WM_LBUTTONUP: // $$$$$ Левая кнопка отжата
		LbuttonClick = false; // $$$$$ Выключим режим выделения клеток. 
		start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
		GenerationFix = calc.Generation;
		break;
	case WM_ERASEBKGND://фон окна должен быть стерт (например, когда окно изменено)
		start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
		GenerationFix = calc.Generation;
		grid.updateBuffer = true; //перерисовываем линии
		return true;

	case WM_MOUSEWHEEL: // $$$$$ Колесо мышки

		// $$$$$ Тут всякий шлак связанный с тем, что мы можем резко крутануть колесо на пять оборотов, а можем его по чуть чуть крутить. Можно изучить, а можно забить.
		wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		for (; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA) // Вверх
		{
			// $$$$$ Увеличиваем масштаб

			grid.AddScale(p.x, p.y);
			grid.updateBuffer = true; //перерисовываем сетку
			InvalidateRect(hWnd, NULL, false);
		}
		for (; wheelDelta < 0; wheelDelta += WHEEL_DELTA) // Вниз
		{
			// $$$$$ Уменьшаем масштаб

			grid.DecScale(p.x, p.y);
			grid.updateBuffer = true; //перерисовываем сетку
			InvalidateRect(hWnd, NULL, false);
		}
		start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
		GenerationFix = calc.Generation;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

		if (LOWORD(wParam) == 555)
		{
			ShellExecute(NULL, L"open", L"http://www.cyberforum.ru/blogs/829006/blog5719.html", NULL, NULL, SW_SHOW);
			//MessageBoxA(NULL, "Hello World!", "Test", MB_OK);
		}
		break;
    }
    return (INT_PTR)FALSE;
}


