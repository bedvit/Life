// Life.cpp : Определяет точку входа для приложения.
#include "stdafx.h"
#include "Life.h"
#include "rle.h"
#include <thread>
#include <Windows.h>
#include "commdlg.h"
#include "shellapi.h"
//#include "Calc.h"

#define MAX_LOADSTRING 100
#define WM_SETFONT     0x0020

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

Grid grid;
Calc calc;
Rle rle;
Point point;

Point size; //Сюда будем присваивать размеры вьюпорта в пикселях и передавать для рисования в Grid.
Point mousePos; // Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение
Point mousePosPoint; //Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение, ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
Point calcPoint;

static int wheelDelta = 0; // требуется для считывания колесика мышки
bool DragEnabled=false; // Для таскания грида правой кнопкой мыши
bool LbuttonClick=false; // Для выделения ячеек левой кнопкой мыши
bool pointDelete=false; // Для выделения/снятия выделения ячеек левой кнопкой мыши, при клике и движении мышки
unsigned int start_time; // = clock();
unsigned int end_time; // = clock(); // конечное время
unsigned int search_time; // = end_time - start_time; // искомое время
unsigned int pre_time; //
unsigned int generation_time; //
unsigned int start_timeNew;
unsigned int search_timeNew;
unsigned int GenerationFix;
long step;

HWND hWndEdit1;
HWND hWndEdit2;
HWND hWndEdit3;
HWND hWndEdit4;
wchar_t* buf = new wchar_t[255];
wchar_t *end;
wchar_t szFileName[MAX_PATH] = L"";
OPENFILENAME ofn;
wchar_t fileOut[MAX_PATH] = L"Life.rle";
wchar_t buffer[MAX_PATH]; //результат для инфо панели
wchar_t bufferTmp[255]; //результат для инфо панели
char vOutChar[255];
static std::wstring nameWin;
static std::wstring nameWinNew;

HFONT hFont = CreateFont(16, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

HBITMAP bitmap;

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
   GetWindowText(hWnd, buffer, 255); //заголовок
   nameWin = std::wstring(buffer);
   
   //создаем свое меню
   RECT rect = { 0 };
  // GetWindowRect(hWnd, &rect); //в координатах экрана
	GetClientRect(hWnd, &rect); //в координатах пользовательского окна
	hWndEdit1 = CreateWindowEx(WS_EX_LEFT, L"Edit", L"0000", WS_CHILD | WS_VISIBLE, rect.right - 100, 230, 100, 14, hWnd, (HMENU)NULL, hInstance, NULL);
	hWndEdit2 = CreateWindowEx(WS_EX_LEFT, L"Edit", L"1", WS_CHILD | WS_VISIBLE, rect.right - 100, 230, 100, 14, hWnd, (HMENU)NULL, hInstance, NULL);
	hWndEdit3 = CreateWindowEx(WS_EX_LEFT, L"Edit", L"ВЫКЛ", WS_CHILD | WS_VISIBLE| WS_DISABLED, rect.right - 100, 230, 100, 14, hWnd, (HMENU)NULL, hInstance, NULL);
	hWndEdit4 = CreateWindowEx(WS_EX_LEFT, L"Edit", L"ВЫКЛ", WS_CHILD | WS_VISIBLE | WS_DISABLED, rect.right - 100, 230, 100, 14, hWnd, (HMENU)NULL, hInstance, NULL);

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
	
	
	// координаты мыши на Экране
	long xPos = LOWORD(lParam);
	long yPos = HIWORD(lParam);

	// Получим координаты мыши в рамках клиентского окна 
	// Клиентское окно или вьюпорт это рабочая область формы (без рамки, заголовка и меню).

	POINT p;
	p.x = xPos;
	p.y = yPos;
	ScreenToClient(hWnd, &p); // Спец функция для этого 

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
				DestroyWindow(hWndEdit1);
				DestroyWindow(hWndEdit2);
				DestroyWindow(hWndEdit3);
				DestroyWindow(hWndEdit4);
                DestroyWindow(hWnd);
                break;
			case IDM_START:
				GetWindowTextW(hWndEdit1, buf, 255); //забираем данные о замедлении из пользовательского меню
				SetTimer(hWnd, 123, wcstol(buf, &end, 10), NULL);
				start_time = clock()- search_time;
				start_timeNew = clock();
				GenerationFix = calc.Generation;
				GetWindowTextW(hWndEdit2, buf, 255); //забираем данные о количестве поколений на один шаг
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
				//заголовок
				nameWinNew = L"* - " + nameWin;
				SetWindowText(hWnd, nameWinNew.c_str());
				////
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
					GetClientRect(hWnd, &rect);// Узнаем размеры клиентского окна.
					KillTimer(hWnd, 123);
					calc.DelLife();
					search_time = 0;
					GenerationFix = 0;
					grid.position = { 0,0 }; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях
					//ShowWindow(hWnd, SW_MAXIMIZE);//Максимизирует указанное окно.
					rle.Load(ofn.lpstrFile, calc, rect, grid);
				}
				//заголовок
				nameWinNew = std::wstring(ofn.lpstrFile);
				nameWinNew = nameWinNew.substr(nameWinNew.find_last_of(L"\\") + 1, std::wstring::npos);
				nameWinNew = nameWinNew + L" - " + nameWin;
				SetWindowText(hWnd, nameWinNew.c_str());
				////
				grid.updateBuffer = true; //перерисовываем сетку
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;
			case IDM_SAVE:
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize =  OPENFILENAME_SIZE_VERSION_400; // SEE NOTE BELOW
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = L"Text Files (*.rle)\0*.rle\0All Files (*.*)\0*.*\0";
				//ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFile = fileOut;
				ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
				ofn.lpstrDefExt = L"rle";
				
				if (GetSaveFileName(&ofn))
				{
					rle.Save(ofn.lpstrFile, calc);
					//заголовок
					nameWinNew = std::wstring(ofn.lpstrFile);
					nameWinNew = nameWinNew.substr(nameWinNew.find_last_of(L"\\") + 1, std::wstring::npos);
					nameWinNew = nameWinNew + L" - " + nameWin;
					SetWindowText(hWnd, nameWinNew.c_str());
					////
				}
				break;
			case IDM_RETURN_TO_ZERO:
				grid.position = { 0,0 };
				grid.updateBuffer = true; //перерисовываем сетку
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;

			case IDM_ZOOM:
				grid.zoom = true;
				start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
				GenerationFix = calc.Generation;//пересчитываем скорость поколений из-за задержки
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;
			case IDM_AUTOZOOM:
				grid.autoZoom= !grid.autoZoom;
				if (grid.autoZoom) SetWindowTextW(hWndEdit3, L"ВКЛ"); else  SetWindowTextW(hWndEdit3, L"ВЫКЛ");
				start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
				GenerationFix = calc.Generation;//пересчитываем скорость поколений из-за задержки
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;

			case IDM_INFO:
				grid.updateInfo = !grid.updateInfo;
				grid.updateBuffer = true; //перерисовываем сетку
				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
				break;

			case IDM_AREALIFE:
				grid.areaLife = !grid.areaLife;
				grid.updateBuffer = true;
				if (grid.areaLife) SetWindowTextW(hWndEdit4, L"ВКЛ"); else  SetWindowTextW(hWndEdit4, L"ВЫКЛ");
				start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
				GenerationFix = calc.Generation;//пересчитываем скорость поколений из-за задержки
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
	case WM_PAINT: // Событие прорисовки. Вызывается системой когда окно нужно перерисовать. Например если мы его растянули и т.д. Мы сами можешь попросить систему вызвать это событие.(см. ниже) 
		{
			//  Узнаем размеры клиентского окна.
			RECT rect;
			GetClientRect(hWnd, &rect);

			size.x = rect.right - rect.left+1;// + 1; //ширина
			size.y = rect.bottom - rect.top+1;// + 1; //высота
			bool update=false;

			if (grid.autoZoom || grid.zoom) //автомасштабирование
			{
				//автомасштабирование
				double scX = (double)rect.right / ((calc.AreaXmax - calc.AreaXmin + 1));
				double scY = (double)rect.bottom / ((calc.AreaYmax - calc.AreaYmin + 1));
				if (scX > scY) scX = scY;// масштаб по макс стороне шаблона
				if (scX > 32) scX = 32; //макс 32 пикселей
				long scalePointTmp = grid.scalePoint;
					double scale = 33;
					if (scX < 1) scX = (-1.00 / scX - 1);
					while (scalePointTmp != scX)//подгоняем масштаб до степени двойки
					{
						if (scale == 1) scale = -2;
						else if (scale <= -32) scale = scale * 2;
						else scale--;

						if (scale <= scX)
						{
							scalePointTmp = scale;
							scX = scalePointTmp;
						}
					}
					if (grid.scalePoint != scalePointTmp)//перерисовываем если изменился масштаб или определяется ареал по живым
					{
						grid.scalePoint = scalePointTmp;
						grid.updateBuffer = true; //обновляем буфер
					}

					Point positionTmp;
					if (grid.scalePoint < 1)//корректируем координаты сетки
					{
						positionTmp = { calc.AreaXmin / grid.scalePoint + 1, calc.AreaYmin / grid.scalePoint + 1 };
					}
					else
					{
						positionTmp = { -calc.AreaXmin*grid.scalePoint, -calc.AreaYmin*grid.scalePoint };
					}

					if (grid.position.x!=positionTmp.x || grid.position.y!= positionTmp.y)
					{
						grid.position.x = positionTmp.x;
						grid.position.y = positionTmp.y;
						grid.updateBuffer = true; //обновляем буфер
					}
				grid.zoom = false;
			}

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);// hdc это инструмент для рисования в клиентской области окна
			HDC hMemDC = CreateCompatibleDC(hdc); //двойная буферизация
			HGDIOBJ oldBmp;
			if (grid.updateBuffer || grid.areaLife)
			{
				BITMAPINFO bi;
				ZeroMemory(&bi, sizeof(BITMAPINFO));
				bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bi.bmiHeader.biWidth = size.x;
				bi.bmiHeader.biHeight = -size.y; //если отрицательное - слева вверху, положительное слева снизу
				bi.bmiHeader.biPlanes = 1;
				bi.bmiHeader.biBitCount = 32;
				bi.bmiHeader.biCompression = BI_RGB;

				DeleteObject(bitmap);
				bitmap = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, (VOID**)&grid.lpBitmapBits, NULL, 0);
				oldBmp = SelectObject(hMemDC, bitmap);
				PatBlt(hMemDC, 0, 0, size.x, size.y, WHITENESS);//закрашиваем прямоугольник белым фоном
				grid.Draw(rect, calc.LifePoint, calc.AreaXmin, calc.AreaYmin, calc.AreaXmax, calc.AreaYmax);
				if (!grid.areaLife) grid.updateBuffer = false; //буфер обновлен
			}
			else
			{
				oldBmp = SelectObject(hMemDC, bitmap);
			}


			if (grid.updateInfo)
			{
				//ИНФО ПАНЕЛЬ
				SelectObject(hMemDC, hFont);
				SetTextColor(hMemDC, RGB(0, 0, 0));
				long Xstart = rect.right - 100; //для авто-выравнивания
				long Ystart = 0;
				long break1 = 15;
				long break2 = 25;

				PatBlt(hMemDC, Xstart, Ystart, 100, 475, WHITENESS);//закрашиваем прямоугольник белым фоном

				Ystart += 0;//Масштаб
				TextOut(hMemDC, Xstart, Ystart, L"Масштаб", 7);
				Ystart += break1;
				if (grid.scalePoint < 1)
				{
					_itow_s(-grid.scalePoint, buffer, 255, 10);
				}
				else
				{
					_itow_s(grid.scalePoint, buffer, 255, 10);
				}
				int iEnd = wcslen(buffer) + 1; //колво символов с '\0'
				if (grid.scalePoint < 1)
				{
					for (int i = 0; i < iEnd - 1; i++)
					{
						bufferTmp[i] = buffer[i];
					}
					bufferTmp[iEnd - 1] = L':';
					bufferTmp[iEnd] = L'1';
					bufferTmp[iEnd + 1] = L'\0';
				}
				else
				{
					bufferTmp[0] = L'1';
					bufferTmp[1] = L':';
					for (int i = 2; i < iEnd + 2; i++)
					{
						bufferTmp[i] = buffer[i - 2];
					}
				}
				TextOut(hMemDC, Xstart, Ystart, bufferTmp, wcsnlen(bufferTmp,255));


				Ystart += break2; //Координаты
				TextOut(hMemDC, Xstart, Ystart, L"Координаты", 10);
				Ystart += break1;
				TextOut(hMemDC, Xstart, Ystart, L"X:", 2);
				Point calcPoint = grid.GetCell(mousePos);
				_itow_s(calcPoint.x, buffer, 255, 10);
				TextOut(hMemDC, Xstart+10, Ystart, buffer, wcsnlen(buffer, 255));
				Ystart += break1;
				TextOut(hMemDC, Xstart, Ystart, L"Y:", 2);
				_itow_s(calcPoint.y, buffer, 255, 10);
				TextOut(hMemDC, Xstart+10, Ystart, buffer, wcsnlen(buffer, 255));


				Ystart += break2;//Ареал, min - max
				TextOut(hMemDC, Xstart, Ystart, L"Ареал", 5);
				Ystart += break1;
				TextOut(hMemDC, Xstart, Ystart, L"Xmin:", 5);
				_i64tow_s(calc.AreaXmin, buffer, 255, 10);
				TextOut(hMemDC, Xstart+33, Ystart, buffer, wcsnlen(buffer, 255));
				Ystart += break1;
				TextOut(hMemDC, Xstart, Ystart, L"Ymin:", 5);
				_itow_s(calc.AreaYmin, buffer, 255, 10);
				TextOut(hMemDC, Xstart+33, Ystart, buffer, wcsnlen(buffer, 255));
				Ystart += break1;
				TextOut(hMemDC, Xstart, Ystart, L"Xmax:", 5);
				_itow_s(calc.AreaXmax, buffer, 255, 10);
				TextOut(hMemDC, Xstart+33, Ystart, buffer, wcsnlen(buffer, 255));
				Ystart += break1;
				TextOut(hMemDC, Xstart, Ystart, L"Ymax:", 5);
				_itow_s(calc.AreaYmax, buffer, 255, 10);
				TextOut(hMemDC, Xstart+33, Ystart, buffer, wcsnlen(buffer, 255));


				Ystart += break2;//Население
				TextOut(hMemDC, Xstart, Ystart, L"Население", 9);
				Ystart += break1;
				_itow_s(calc.Population, buffer, 255, 10);
				TextOut(hMemDC, Xstart, Ystart, buffer, wcsnlen(buffer, 255));


				Ystart += break2;//Поколение
				TextOut(hMemDC, Xstart, Ystart, L"Поколение", 9);
				Ystart += break1;
				_itow_s(calc.Generation, buffer, 255, 10);
				TextOut(hMemDC, Xstart, Ystart, buffer, wcsnlen(buffer, 255));


				Ystart += break2;//Таймер, сек.
				TextOut(hMemDC, Xstart, Ystart, L"Таймер, сек.", 12);
				Ystart += break1;
				_gcvt_s(vOutChar, sizeof(vOutChar), ((double)search_time / 1000), 5);
				//_fcvt_s(buffer, _CVTBUFSIZE,search_time/1000, 5, &decimal, &sign); //для char
				mbstowcs_s(NULL, buffer, sizeof(buffer) / 2, vOutChar, sizeof(vOutChar));
				TextOut(hMemDC, Xstart, Ystart, buffer, wcsnlen(buffer, 255));


				Ystart += break2;//Поколений/сек.
				TextOut(hMemDC, Xstart, Ystart, L"Поколений/сек.", 14);
				Ystart += break1;
				double out = 0.0;
				if (search_timeNew != 0) out = ((double)(calc.Generation - GenerationFix) * 1000 / search_timeNew);// накопительным итогом на каждый запуск
				_gcvt_s(vOutChar, sizeof(vOutChar), out, 5);
				mbstowcs_s(NULL, buffer, sizeof(buffer) / 2, vOutChar, sizeof(vOutChar));
				TextOut(hMemDC, Xstart, Ystart, buffer, wcsnlen(buffer, 255));
		

				Ystart += break2;//Замедление
				TextOut(hMemDC, Xstart, Ystart, L"Замедление, мс.", 15);
				Ystart += break1;
				MoveWindow(hWndEdit1, Xstart, Ystart, 100, 14, true);


				Ystart += break2;//Шаг поколений 
				TextOut(hMemDC, Xstart, Ystart, L"Шаг поколений", 13);
				Ystart += break1;
				MoveWindow(hWndEdit2, Xstart, Ystart, 100, 14, true);


				Ystart += break2;//Автомасштаб
				TextOut(hMemDC, Xstart, Ystart, L"Автомасштаб", 11);
				Ystart += break1;
				MoveWindow(hWndEdit3, Xstart, Ystart, 100, 14, true);


				Ystart += break2;//Ареал по живым
				TextOut(hMemDC, Xstart, Ystart, L"Ареал по живым", 14);
				Ystart += break1;
				MoveWindow(hWndEdit4, Xstart, Ystart, 100, 14, true);
				//ИНФО ПЕНЕЛЬ
			}
			else
			{
			MoveWindow(hWndEdit1, -100, -100, 100, 14, true);//убираем с экрана
			MoveWindow(hWndEdit2, -100, -100, 100, 14, true);
			MoveWindow(hWndEdit3, -100, -100, 100, 14, true);
			MoveWindow(hWndEdit4, -100, -100, 100, 14, true);
			}

			BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, 0, 0, SRCCOPY);
			SelectObject(hMemDC, oldBmp);

			DeleteDC(hMemDC);
			DeleteObject(oldBmp);
			DeleteDC(hdc);
			EndPaint(hWnd, &ps);

        }
        break;

    case WM_DESTROY: //подчищаем память
		delete[] buf;
		DeleteObject(bitmap);
		PostQuitMessage(0);
        break;

	case WM_LBUTTONDOWN: // $ Левая кнопка нажата
		if (grid.scalePoint >= 1) // не рисуем при масштабе меньше 1 
		{
			LbuttonClick = true; // Включим режим выделения клеток
			mousePos.x = xPos;  // Запомним координаты мыши
			mousePos.y = yPos;
			calcPoint = grid.GetCell(mousePos); // ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
			if (calc.Contains(calcPoint, calc.LifePoint)) pointDelete = true; else pointDelete = false; //смотрим есть ли такой элемент
			mousePosPoint.x = calcPoint.x;
			mousePosPoint.y = calcPoint.y;
			calc.Insert(calcPoint, calc.LifePoint, pointDelete,grid);
			InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
		}
		break;

	case WM_RBUTTONDOWN: // Правая кнопка нажата
		DragEnabled = true; // Включим режим таскания сетки
		mousePos.x = xPos;  // Запомним координаты мыши
		mousePos.y = yPos;

		break;

	case WM_MOUSEMOVE:  //  Мышка двигается 
		
		// используем TrackMouseEvent, при выходе за пределы пользовательского окна
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = hWnd;
		tme.dwFlags = TME_LEAVE;
		TrackMouseEvent(&tme);

		if (DragEnabled) // При этом зажата правая кнопка т.к. включен режим таскания
		{
			// двигаем грид на дельту с прошлого события
			grid.Move(xPos - mousePos.x, yPos - mousePos.y);
			mousePos.x = xPos;
			mousePos.y = yPos;
			grid.updateBuffer = true; //перерисовываем сетку
			InvalidateRect(hWnd, NULL, false);
		}
		else if (LbuttonClick)  //включен режим выделения
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
		else
		{
			mousePos.x = xPos;  // Запомним координаты мыши
			mousePos.y = yPos;
			InvalidateRect(hWnd, NULL, false);//перерисовать клиентское окно
		}
		break;
	case WM_MOUSELEAVE: //Сообщение WM_MOUSELEAVE посылается в окно тогда, когда курсор оставляет рабочую область окна, заданную при предшествующем вызове функции TrackMouseEvent.
		LbuttonClick=false; //отключаем выделение
		DragEnabled = false; // Выключим режим таскания. Если не выключить, сетка будет вечно ходить за мышкой.
		break;

	case WM_RBUTTONUP: // Правая кнопка отжата
		DragEnabled = false; // Выключим режим таскания. Если не выключить, сетка будет вечно ходить за мышкой.
		start_timeNew = clock(); //пересчитываем скорость поколений из-за задержки
		GenerationFix = calc.Generation;
		break;
	case WM_LBUTTONUP: // Левая кнопка отжата
		LbuttonClick = false; // Выключим режим выделения клеток. 
		start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
		GenerationFix = calc.Generation;
		break;
	case WM_ERASEBKGND://фон окна должен быть стерт (например, когда окно изменено)
		return true;
	case WM_SIZE: //изменения размеров окна
		start_timeNew = clock();//пересчитываем скорость поколений из-за задержки
		GenerationFix = calc.Generation;
		grid.updateBuffer = true; //перерисовываем линии
		break;

	case WM_MOUSEWHEEL: // Колесо мышки

		// Тут всякий шлак связанный с тем, что мы можем резко крутануть колесо на пять оборотов, а можем его по чуть чуть крутить.
		wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		for (; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA) // Вверх
		{
			//Увеличиваем масштаб
			grid.AddScale(p.x, p.y);
			grid.updateBuffer = true; //перерисовываем сетку
			InvalidateRect(hWnd, NULL, false);
		}
		for (; wheelDelta < 0; wheelDelta += WHEEL_DELTA) // Вниз
		{
			// Уменьшаем масштаб
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


