﻿// Life.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Life.h"
#include "Grid.h"
#include "Calc.h"
//#include "Windef.h"
#include <thread>

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
// $$$$$ Класс point написал тоже я. Это просто пара целых переменных x, y. Им можно задавать координаты ячейки грида, координаты пикселя, разность между двумя точками. Полезный короче. 
// В стандартной библиотеке есть такой класс, но я хотел чтобы ты заценил насколько он простой. Потом я его убью и буду юзать типовой.
POINT size; // $$$$$ Сюда будем присваивать размеры вьюпорта в пикселях и передавать для рисования в Grid.
POINT mousePos; // $$$$$ Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение
POINT mousePosPoint; //Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение, ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
//HWND hWnd;

bool RunCalc=false; //запустить расчет жизни 
bool CalcEnd=false; //вычисления закончены - готов расчет нового поколения
static int wheelDelta = 0; // $$$$$ требуется для считывания колесика мышки
bool DragEnabled; // $$$$$ Для таскания грида правой кнопкой мыши
bool LbuttonClick; // $$$$$ Для выделения ячеек левой кнопкой мыши
bool LifeInvert; // $$$$$ Для выделения/снятия выделения ячеек левой кнопкой мыши, при клике и движении мышки

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
   //GetWindowRect(hWnd, &rect); в координатах экрана
   GetClientRect(hWnd, &rect); //в координатах пользовательского окна
   //HWND hWndEx = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, L"STATIC", L"Panel 1", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | SS_SUNKEN, 0, 0, 100, rect.bottom, hWnd, (HMENU)NULL, GetModuleHandle(0), (LPVOID)NULL);
   HWND Panel1 = CreateWindowEx(WS_EX_WINDOWEDGE , L"STATIC", L"Panel 1",  WS_CHILD | WS_VISIBLE , 100, 0, 1000, 20, hWnd, (HMENU)NULL, GetModuleHandle(0), (LPVOID)NULL);
   
   //HWND button1 = CreateWindow(L"button", L"Пуск", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 50, 20, Panel1, (HMENU)NULL, hInstance, NULL);
   //HWND button2 = CreateWindow(L"button", L"Стоп", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 0, 50, 20, Panel1, (HMENU)NULL, hInstance, NULL);
   
   HWND button1 = CreateWindowEx(WS_EX_RIGHT,L"Static", L"Поколений=", WS_CHILD | WS_VISIBLE, 0, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button2 = CreateWindowEx(WS_EX_LEFT,L"Static", L"2565465465", WS_CHILD | WS_VISIBLE, 100, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button3 = CreateWindowEx(WS_EX_RIGHT,L"Static", L"Популяция=", WS_CHILD | WS_VISIBLE, 200, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button4 = CreateWindowEx(WS_EX_LEFT,L"Static", L"2565465465", WS_CHILD | WS_VISIBLE, 300, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button5 = CreateWindowEx(WS_EX_RIGHT,L"Static", L"Масштаб=", WS_CHILD | WS_VISIBLE, 400, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button6 = CreateWindowEx(WS_EX_LEFT,L"Static", L"1:15", WS_CHILD | WS_VISIBLE, 500, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button7 = CreateWindowEx(WS_EX_RIGHT,L"Static", L"Замедление, млс", WS_CHILD | WS_VISIBLE, 600, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button8 = CreateWindowEx(WS_EX_LEFT,L"Edit", L"000", WS_CHILD | WS_VISIBLE | WS_BORDER, 700, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button9 = CreateWindowEx(WS_EX_RIGHT,L"Static", L"ХУ=", WS_CHILD | WS_VISIBLE, 800, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);
   HWND button10 = CreateWindowEx(WS_EX_LEFT,L"Static", L"-0-0", WS_CHILD | WS_VISIBLE, 900, 0, 100, 20, Panel1, (HMENU)1001, hInstance, NULL);


   //Функцией Edit_GetText или GetWindowText получаешь нужный текст.
//	   http://msdn.microsoft.com/en-us/libr...(v=vs.85).aspx


   HFONT h_font = CreateFont(-20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");
   HWND button100 = CreateWindowEx(0x00000000, L"Button", L"open", 0x50012F00, 24, 24, 96, 32, hWnd, (HMENU)1000, hInstance, NULL);
   SendMessage(button100, WM_SETFONT, (WPARAM)h_font, TRUE);

   //HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //задаём сплошную кисть, закрашенную цветом RGB - черный
   //GetClientRect(hWndEx, &rect); //в координатах пользовательского окна
   //FillRect(hWndEx, &rect, s);
   //SetLayeredWindowAttributes(hWndEx, RGB(0, 0, 0), 0, LWA_COLORKEY); //прозрачность



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
                DestroyWindow(hWnd);
                break;
			case IDM_START:
				RunCalc = true;
				calc.RunLife(CalcEnd);
				SetTimer(hWnd, 123, 0, NULL);
				break;
			case IDM_STOP:
				RunCalc =false;
				KillTimer(hWnd, 123);
				break;
			case IDM_NEW:
				calc.DelLife();
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;
			caseIDM_OPEN:	
				break;
			case IDM_SAVE:
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_TIMER:
		{
			//блок для отслеживание вычисления в параллельных потоках
			if (CalcEnd)
			{
				InvalidateRect(hWnd, NULL, false);//перерисовать клиентское окно ели готово новое поколение жизни.
			}
			if (RunCalc && CalcEnd)
			{
				CalcEnd = false;
				calc.RunLife(CalcEnd);
			}
		}
		break;
	case WM_PAINT: // $$$ Событие прорисовки. Вызывается системой когда окно нужно перерисовать. Например если мы его растянули и т.д. Мы сами можешь попросить систему вызвать это событие.(см. ниже) 
		{
			// $$$$$ Узнаем размеры клиентского окна.
			RECT rect;
			GetClientRect(hWnd, &rect);

			size.x = rect.right - rect.left + 1;
			size.y = rect.bottom - rect.top + 1;
			
			//HWND ff = FindWindowExA(hWnd, NULL, "STATIC", "Panel 1");
			//SetWindowPos(FindWindowEx(hWnd, NULL, L"STATIC", L"Panel 1"), HWND_TOPMOST, 100, 0, 1000, 20, SWP_NOREDRAW); //перерисовываем дочернее окно


			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);// hdc это инструмент для рисования в клиентской области окна
			HDC hMemDC = CreateCompatibleDC(hdc);
			
			HBITMAP hScreen = CreateCompatibleBitmap(hdc, size.x, size.y);
			HBITMAP oldBmp = (HBITMAP)SelectObject(hMemDC, hScreen);
			
			PatBlt(hMemDC, 0, 0, size.x, size.y, WHITENESS);

			grid.Draw(hMemDC, size);// $$$$$ Скажем гриду, чтобы нарисовал себя в рамках размеров клиентского окна
			
			grid.FillRectangle(hMemDC, calc);//Заполняем клетки
			
			//ValidateRect(hWnd, NULL); //обновляем окно - НУЖНО ЛИ??? ХЗ

			BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, 0, 0, SRCCOPY);

			SelectObject(hMemDC, oldBmp);
			DeleteObject(hScreen);
			DeleteDC(hMemDC);
			EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

		case WM_LBUTTONDOWN: // $ Левая кнопка нажата

			LbuttonClick = true; // $$$$$ Включим режим выделения клеток
			mousePos.x = xPos;  // $$$$$ Запомним координаты мыши
			mousePos.y = yPos;
			POINT calcPoint = grid.GetCell(mousePos); // ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
			if (calc.Contains(calcPoint, calc.LifePoint)) LifeInvert = true; else LifeInvert = false; //смотрим есть ли такой элемент
			//if(mousePosPoint.x == calcPoint.x && mousePosPoint.y == calcPoint.y) LifeInvert = !LifeInvert;//если ккординаты совпадаем делаем инвертирование цвета
			mousePosPoint.x = calcPoint.x;
			mousePosPoint.y = calcPoint.y;
			calc.Insert(calcPoint, LifeInvert, calc.LifePoint);
			InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
			break;

		case	WM_RBUTTONDOWN: // $ Правая кнопка нажата

			DragEnabled = true; // $$$$$ Включим режим таскания сетки
			mousePos.x = xPos;  // $$$$$ Запомним координаты мыши
			mousePos.y = yPos;

			break;

		case WM_MOUSEMOVE:  // $$$$$ Мышка двигается 

			if (DragEnabled) // $$$$$ При этом зажата правая кнопка т.к. включен режим таскания
			{
				// $$$$$ двигаем грид на дельту с прошлого события
				grid.Move(xPos - mousePos.x, yPos - mousePos.y);
				mousePos.x = xPos;
				mousePos.y = yPos;
				InvalidateRect(hWnd, NULL, false);
			}
			else if (LbuttonClick)
			{
				long xPosTmp = xPos - mousePos.x;//разница от текущих координат и записанных в координатах экрана
				long yPosTmp = yPos - mousePos.y;
				long MaxPosTmp;// пропуски мышки, которые мы будем обсчитывать
				
				if (abs(xPosTmp) > abs(yPosTmp)) MaxPosTmp = abs(xPosTmp); else MaxPosTmp = abs(yPosTmp);//макс пропуск в пикселях
				//if (MaxPosTmp==0) break; //для одинаковых координат выходим из цикла
				long ColPosTmp = (MaxPosTmp / grid.scale)+1;//количество циклов расчета координат по этому пропуску с учетом масштаба

				POINT start = mousePos;

				for (long i = 0; i < ColPosTmp; i++)
				{
					mousePos.x = start.x + (float)xPosTmp/ ColPosTmp*(i+1);
					mousePos.y = start.y + (float)yPosTmp / ColPosTmp * (i + 1);

					POINT calcPoint = grid.GetCell(mousePos); // ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
					if (mousePosPoint.x != calcPoint.x || mousePosPoint.y != calcPoint.y)
					{
						mousePosPoint.x = calcPoint.x;
						mousePosPoint.y = calcPoint.y;
						calc.Insert(calcPoint, LifeInvert, calc.LifePoint);
						
					}
				}
			mousePos.x = xPos;
			mousePos.y = yPos;
			InvalidateRect(hWnd, NULL, false);//перерисовать клиентское окно
			}
			break;

		case WM_RBUTTONUP: // $$$$$ Правая кнопка отжата
			DragEnabled = false; // $$$$$ Выключим режим таскания. Если не выключить, сетка будет вечно ходить за мышкой.
			break;
		case WM_LBUTTONUP: // $$$$$ Левая кнопка отжата
			LbuttonClick = false; // $$$$$ Выключим режим выделения клеток. 
			break;
		case WM_ERASEBKGND:

			return true;

		case WM_MOUSEWHEEL: // $$$$$ Колесо мышки

			// $$$$$ Тут всякий шлак связанный с тем, что мы можем резко крутануть колесо на пять оборотов, а можем его по чуть чуть крутить. Можно изучить (я понял как работает, например), а можно забить.
			wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
			for (; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA) // Вверх
			{
				// $$$$$ Увеличиваем масштаб

				grid.AddScale(p.x, p.y);
				InvalidateRect(hWnd, NULL, false);
			}
			for (; wheelDelta < 0; wheelDelta += WHEEL_DELTA) // Вниз
			{
				// $$$$$ Уменьшаем масштаб

				grid.DecScale(p.x, p.y);
				InvalidateRect(hWnd, NULL, false);
			}
			break;



		default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//DWORD dwExStyle = WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME;
//DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | SS_SUNKEN;
//Handle = CreateWindowEx(dwExStyle, "STATIC", "Panel 1", dwStyle, 0, 0, 300, 300, hwndOwner, (HMENU)NULL, hInstance, (LPVOID)NULL);

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
        break;
    }
    return (INT_PTR)FALSE;
}
