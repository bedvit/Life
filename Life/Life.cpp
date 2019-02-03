﻿// Life.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Life.h"
//#include "Grid.h"
//#include "Calc.h"
//#include "Point.h"
//#include "rle.h"
////#include "Windef.h"
//#include <thread>
//#include "Windows.h"
//#include "commdlg.h"
//
//#include <d2d1.h>
//
//#define MAX_LOADSTRING 100
//#define WM_SETFONT     0x0020
//
//// Глобальные переменные:
//HINSTANCE hInst;                                // текущий экземпляр
//WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
//WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
//
//// $$$$$ Это глобальная переменная класса Grid, который я написал. 
//// Этот класс поможет нам а) рисовать сетку б) красить в ней ячейки.
//// Он позволит нам думать что у нас есть бесконечное поле точек. 
//
//Grid grid;
//Calc calc;
//Rle rle;
//Point point;
//
//// $$$$$ Класс point написал тоже я. Это просто пара целых переменных x, y. Им можно задавать координаты ячейки грида, координаты пикселя, разность между двумя точками. Полезный короче. 
//// В стандартной библиотеке есть такой класс, но я хотел чтобы ты заценил насколько он простой. Потом я его убью и буду юзать типовой.
//Point size; // $$$$$ Сюда будем присваивать размеры вьюпорта в пикселях и передавать для рисования в Grid.
//Point mousePos; // $$$$$ Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение
//Point mousePosPoint; //Здесь будем хранить позицию мышки с прошлого события, чтобы палить смещение, ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
//Point calcPoint;
//					 //HWND hWnd;
//
////bool RunCalc=false; //запустить расчет жизни 
////bool CalcEnd=false; //вычисления закончены - готов расчет нового поколения
////bool Pause = false; //когда отрисовываем экран calc не считает
//static int wheelDelta = 0; // $$$$$ требуется для считывания колесика мышки
//bool DragEnabled; // $$$$$ Для таскания грида правой кнопкой мыши
//bool LbuttonClick; // $$$$$ Для выделения ячеек левой кнопкой мыши
//bool pointDelete; // $$$$$ Для выделения/снятия выделения ячеек левой кнопкой мыши, при клике и движении мышки
//unsigned int start_time; // = clock();
//unsigned int end_time; // = clock(); // конечное время
//unsigned int search_time; // = end_time - start_time; // искомое время
//unsigned int pre_time; //
//unsigned int generation_time; //
//unsigned int start_timeNew;
//unsigned int search_timeNew;
//unsigned int GenerationFix;
//
//HWND hWndEdit;
////открыть файл
//wchar_t* buf = new wchar_t[255];
//wchar_t *end;
//wchar_t szFileName[MAX_PATH] = L"";
//OPENFILENAME ofn;
//wchar_t fileOut[MAX_PATH] = L"Life.rle";
//wchar_t buffer[255]; //результат для инфо панели
//char vOutChar[255];
//HFONT hFont = CreateFont(16, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
//
////DirectX2D
//template <class T>
//void SafeRelease(T **ppT)
//{
//	if (*ppT)
//	{
//		(*ppT)->Release();
//		*ppT = NULL;
//	}
//}
//////////////////////////////
//
//// Отправить объявления функций, включенных в этот модуль кода:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: Разместите код здесь.
//
//    // Инициализация глобальных строк
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_LIFE, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // Выполнить инициализацию приложения:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LIFE));
//
//    MSG msg;
//
//    // Цикл основного сообщения:
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    return (int) msg.wParam;
//}
//
//
//
////
////  ФУНКЦИЯ: MyRegisterClass()
////
////  ЦЕЛЬ: Регистрирует класс окна.
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style          = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc    = WndProc;
//    wcex.cbClsExtra     = 0;
//    wcex.cbWndExtra     = 0;
//    wcex.hInstance      = hInstance;
//    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIFE));
//    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
//    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LIFE);
//    wcex.lpszClassName  = szWindowClass;
//    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}
//
////
////   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
////
////   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
////
////   КОММЕНТАРИИ:
////
////        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
////        создается и выводится главное окно программы.
////
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
//
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//   if (!hWnd) return FALSE;
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//   
//   //создаем свое меню
//   RECT rect = { 0 };
//   //GetWindowRect(hWnd, &rect); в координатах экрана
//  GetClientRect(hWnd, &rect); //в координатах пользовательского окна
//   
//   //HWND Panel1 = CreateWindowEx(WS_EX_WINDOWEDGE , L"STATIC", L"Panel 1",  WS_CHILD | WS_VISIBLE , 100, 0, 1000, 20, hWnd, (HMENU)NULL, GetModuleHandle(0), (LPVOID)NULL);
//   //HWND button1 = CreateWindowEx(WS_EX_RIGHT,L"Static", L"Поколений=", WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, Panel1, (HMENU)NULL, hInstance, NULL);
//
//   //Функцией Edit_GetText или GetWindowText получаешь нужный текст.
////	   http://msdn.microsoft.com/en-us/libr...(v=vs.85).aspx
//
// 	hWndEdit = CreateWindowEx(WS_EX_LEFT, L"Edit", L"0000", WS_CHILD | WS_VISIBLE, rect.right - 100, 230, 100, 14, hWnd, (HMENU)NULL, hInstance, NULL);
//   //HFONT h_font = CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 1, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");
//   //LRESULT rr=SendMessage(button100, WM_SETFONT, (WPARAM)h_font, TRUE);
//
//
////   PAINTSTRUCT psPanel1;
////   HDC hdcPanel1 = BeginPaint(button1, &psPanel1);
////   
////   HFONT hFont = CreateFont(-5, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
////	   CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
////   SelectObject(hdcPanel1, hFont);
////   LRESULT rr = SendMessage(button1, WM_SETFONT, (WPARAM)hFont, TRUE);
////TextOut(hdcPanel1, 0, 0, L"проверка связи", wcslen(L"проверка связи"));
//
//
//   //HBRUSH s = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //задаём сплошную кисть, закрашенную цветом RGB - черный
//   //GetClientRect(hWndEx, &rect); //в координатах пользовательского окна
//   //FillRect(hWndEx, &rect, s);
//   //SetLayeredWindowAttributes(button100, RGB(0, 0, 0), 0, LWA_COLORKEY); //прозрачность
//
//
//
//   return TRUE;
//}
//
////
////  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
////
////  WM_COMMAND  - обработать меню приложения
////  WM_PAINT    - Отрисовка главного окна
////  WM_DESTROY  - отправить сообщение о выходе и вернуться
////
////
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	
//	
//	// $$$$ координаты мыши на Экране
//	long xPos = LOWORD(lParam);
//	long yPos = HIWORD(lParam);
//
//	// $$$$$ Получим координаты мыши в рамках клиентского окна 
//	// $$$$$ Клиентское окно или вьюпорт это рабочая область формы (без рамки, заголовка и меню).
//
//	Point p;
//	p.x = xPos;
//	p.y = yPos;
//	POINT P;
//	ScreenToClient(hWnd, &P); // $$$$$ Спец функция для этого 
//
//	//wchar_t* buf = new wchar_t[255];
//	////HWND hWndEdit;
//	//wchar_t *end;
//	/////
//	////открыть файл
//	//wchar_t szFileName[MAX_PATH] = L"";
//	//OPENFILENAME ofn;
//	//wchar_t fileOut[MAX_PATH] = L"Life.rle";
//	///
//
//    switch (message)
//    {
//    case WM_COMMAND:
//        {
//            int wmId = LOWORD(wParam);
//            // Разобрать выбор в меню:
//            switch (wmId)
//            {
//            case IDM_ABOUT:
//                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//                break;
//            case IDM_EXIT:
//				DestroyWindow(hWndEdit);
//                DestroyWindow(hWnd);
//                break;
//			case IDM_START:
//					GetWindowTextW(hWndEdit, buf, 255); //забираем данные о замедлении из пользовательского меню
//					SetTimer(hWnd, 123, wcstol(buf, &end, 10), NULL);
//					start_timeNew = clock();
//					start_time = clock()- search_time;
//					GenerationFix = calc.Generation();
//					//RunCalc = true;
//				//if (!RunCalc)
//				//{
//					//hWndEdit = FindWindowEx(hWnd, NULL, _TEXT("Edit"), _TEXT("0000"));
//					//calc.Pause()=Pause;
//					//delete(buf);
//
//					//calc.RunLife(RunCalc, CalcEnd,Pause);
//					//calc.RunLife();
//			/*	}*/
//				break;
//			case IDM_STOP:
//				//RunCalc =false;
//				KillTimer(hWnd, 123);
//				break;
//			case IDM_NEW:
//				//RunCalc = false;
//				KillTimer(hWnd, 123);
//				calc.DelLife();
//				search_time = 0;
//				GenerationFix = 0;
//				grid.position = {0,0}; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях
//				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
//				break;
//			case IDM_OPEN:
//				//RunCalc = false;
//				RECT rect;
//				GetClientRect(hWnd, &rect);// $$$$$ Узнаем размеры клиентского окна.
//				KillTimer(hWnd, 123);
//				calc.DelLife();
//				search_time = 0;
//				GenerationFix = 0;
//				grid.position = { 0,0 }; // позиция начала координат сетки относительно левого верхнего угла окна в пискселях
//
//				ZeroMemory(&ofn, sizeof(ofn));
//				ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
//				ofn.hwndOwner = hWnd;
//				ofn.lpstrFilter = L"Text Files (*.rle)\0*.rle\0All Files (*.*)\0*.*\0";
//				ofn.lpstrFile = szFileName;
//				ofn.nMaxFile = MAX_PATH;
//				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
//				ofn.lpstrDefExt = L"rle";
//				if (GetOpenFileName(&ofn))
//				{
//					// Do something usefull with the filename stored in szFileName 
//					//ShowWindow(hWnd, SW_MAXIMIZE);//Максимизирует указанное окно.
//					rle.Load(ofn.lpstrFile, calc, rect, grid);
//				}
//				InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
//				break;
//			case IDM_SAVE:
//				ZeroMemory(&ofn, sizeof(ofn));
//				ofn.lStructSize =  OPENFILENAME_SIZE_VERSION_400; // SEE NOTE BELOW
//				ofn.hwndOwner = hWnd;
//				ofn.lpstrFilter = L"Text Files (*.rle)\0*.rle\0All Files (*.*)\0*.*\0";
//				ofn.lpstrFile = szFileName;
//				ofn.nMaxFile = MAX_PATH;
//				ofn.lpstrFile = fileOut;
//				ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
//				ofn.lpstrDefExt = L"rle";
//				
//				if (GetSaveFileName(&ofn))
//				{
//					// Do something usefull with the filename stored in szFileName 
//					rle.Save(ofn.lpstrFile, calc);
//				}
//				break;
//            default:
//            return DefWindowProc(hWnd, message, wParam, lParam);
//            }
//        }
//        break;
//
//	case WM_TIMER:
//		{
//			//блок для отслеживание вычисления в параллельных потоках
//			//if (CalcEnd)
//			//{
//				InvalidateRect(hWnd, NULL, false);//перерисовать клиентское окно ели готово новое поколение жизни.
//			//}
//			//if (RunCalc && CalcEnd)
//			//{
//				//CalcEnd = false;
//				//calc.RunLife(RunCalc, CalcEnd, Pause);
//				calc.RunLife();
//				end_time = clock(); // конечное время
//				search_time = end_time - start_time; // искомое время
//				search_timeNew = end_time - start_timeNew;
//				generation_time = end_time - pre_time; //
//				pre_time = end_time; // конечное время
//			//}
//		}
//		break;
//	case WM_PAINT: // $$$ Событие прорисовки. Вызывается системой когда окно нужно перерисовать. Например если мы его растянули и т.д. Мы сами можешь попросить систему вызвать это событие.(см. ниже) 
//		{
//			//// $$$$$ Узнаем размеры клиентского окна.
//			//RECT rect;
//			//GetClientRect(hWnd, &rect);
//
//			//size.x = rect.right - rect.left + 1;
//			//size.y = rect.bottom - rect.top + 1;
//			//
//			PAINTSTRUCT ps;
//			HDC hdc = BeginPaint(hWnd, &ps);// hdc это инструмент для рисования в клиентской области окна
//			HDC hMemDC = CreateCompatibleDC(hdc); //двойная буферизация
//			HBITMAP hScreen = CreateCompatibleBitmap(hdc, size.x, size.y);
//			HBITMAP oldBmp = (HBITMAP)SelectObject(hMemDC, hScreen);
//			PatBlt(hMemDC, 0, 0, size.x, size.y, WHITENESS);
//			////grid.Draw(hMemDC, size);// $$$$$ Скажем гриду, чтобы нарисовал себя в рамках размеров клиентского окна
//			//////grid.FillRectangle(hMemDC, calc);//Заполняем клетки
//			////
//
//
//			//direct2D
//				ID2D1Factory* pD2DFactory = NULL;
//				HRESULT hr = D2D1CreateFactory(
//					D2D1_FACTORY_TYPE_SINGLE_THREADED,
//					&pD2DFactory
//				);
//
//				RECT rc;
//				GetClientRect(hWnd, &rc);
//
//				// Create a Direct2D render target          
//				ID2D1HwndRenderTarget* pRT = NULL;
//				hr = pD2DFactory->CreateHwndRenderTarget(
//					D2D1::RenderTargetProperties(),
//					D2D1::HwndRenderTargetProperties(
//						hWnd,
//						D2D1::SizeU(
//							rc.right - rc.left,
//							rc.bottom - rc.top)
//					),
//					&pRT
//				);
//			
//		
//		ID2D1SolidColorBrush* pBlackBrush = NULL;
//		if (SUCCEEDED(hr))
//		{
//
//			pRT->CreateSolidColorBrush(
//				D2D1::ColorF(D2D1::ColorF::Red),
//				&pBlackBrush
//			);
//		}
//		
//		//grid.FillRectangle2(pRT, pBlackBrush, calc);//Заполняем клетки
//		pRT->BeginDraw();
//		RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
//		std::unordered_map<LONGLONG, Point>::iterator i;
//		
//		for (i = calc.LifePoint.begin(); i != calc.LifePoint.end(); i++)
//		{
//			if (i->second.life)
//			{
//				r.left = i->second.x * grid.scale + grid.position.x; //X-координата верхнего левого угла прямоугольника.
//				r.top = i->second.y * grid.scale + grid.position.y; // i->second.y;//Y-координата верхнего левого угла прямоугольника.
//				r.right = (i->second.x + 1) * grid.scale + grid.position.x;//X-координата нижнего правого угла прямоугольника.
//				r.bottom = (i->second.y + 1) * grid.scale + grid.position.y; //Y-координата нижнего правого угла прямоугольника.
//
//				//pRT->BeginDraw();
//
//				pRT->DrawRectangle(
//					D2D1::RectF(
//						r.left,
//						r.top,
//						r.right,
//						r.bottom),
//					pBlackBrush);
//				//pRT->EndDraw();
//			}
//		}
//		pRT->EndDraw();
//		SafeRelease(&pRT);
//		SafeRelease(&pBlackBrush);
//		SafeRelease(&pD2DFactory);
//		
//		
//		
//		
//		///////////////////////////////////////////////////////////
//			
//			// ИНФО ПАНЕЛЬ
//			//RECT rectTxt; //координаты текста
//
//			//
//			////wchar_t buffer[255]; //результат для инфо панели
//			////HFONT hFont = CreateFont(16,0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
//			//SelectObject(hMemDC, hFont);
//			//SetTextColor(hMemDC, RGB(0, 0, 0));
//			//long Xstart= rect.right - 100; //для авто-выравнивания
//			//long Ystart=0;	
//
//
//			//Ystart += 10;//Масштаб
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Масштаб"), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("1:"), -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
//			//_itow_s(grid.scale, buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//
//
//			//Ystart += 30; //Координаты
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Координаты"), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20; 
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("X:"), -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
//			//Point calcPoint = grid.GetCell(mousePos);
//			//_itow_s(calcPoint.x, buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Y:"), -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
//			//_itow_s(calcPoint.y, buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//
//
//			//Ystart += 30;//Ареал
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Ареал"), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("X:"), -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
//			//_itow_s(calc.AreaXmin(), buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 15;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Y:"), -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
//			//_itow_s(calc.AreaYmin(), buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("X:"), -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
//			//_itow_s(calc.AreaXmax(), buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 15;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Y:"), -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart + 10, Ystart, 0, 0);
//			//_itow_s(calc.AreaYmax(), buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//			//
//
//			//Ystart += 30;//Население
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Население"), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//_itow_s(calc.population, buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP); 
//			//
//			//
//			//Ystart += 30;//Поколение
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Поколение"), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//_itow_s(calc.Generation(), buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//			//
//
//			//Ystart += 30;//Таймер
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Таймер, сек."), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			////SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			////_itow_s(search_time, buffer, 255, 10);
//			////DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//int decimal;
//			//int sign;
//			//_gcvt_s(vOutChar, sizeof(vOutChar), ((double)search_time / 1000), 5);
//			////_fcvt_s(buffer, _CVTBUFSIZE,search_time/1000, 5, &decimal, &sign); //для char
//			//mbstowcs_s(NULL, buffer, sizeof(buffer) / 2, vOutChar, sizeof(vOutChar));
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//
//			//Ystart += 30;//Таймер
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Поколений/сек."), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//int out = 0;
//			////if (generation_time != 0) out = (1000 / (generation_time));// каждый ход новое значение
//			//if(search_timeNew != 0) out=((calc.Generation()- GenerationFix) * 1000 / search_timeNew);// накопительным итогом на каждый запуск
//			////if (calc.Generation() != 0) out = (calc.Generation()*1000/search_time); //накопительно за все время выполнения
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//_itow_s(out, buffer, 255, 10);
//			//DrawText(hMemDC, buffer, -1, &rectTxt, DT_NOCLIP);
//
//
//			//Ystart += 30;//Замедление
//			//SetRect(&rectTxt, Xstart, Ystart, 0, 0);
//			//DrawText(hMemDC, TEXT("Замедление, мс."), -1, &rectTxt, DT_NOCLIP);
//			//Ystart += 20;
//			//MoveWindow(hWndEdit, Xstart, Ystart, 100, 14, TRUE);
//			////ИНФО ПЕНЕЛЬ
//			//
//
//			BitBlt(hdc, 0, 0, size.x, size.y, hMemDC, 0, 0, SRCCOPY);
//			SelectObject(hMemDC, oldBmp);
//			DeleteObject(hScreen);
//			DeleteObject(oldBmp);
//			DeleteDC(hdc);
//			DeleteDC(hMemDC);
//			EndPaint(hWnd, &ps);
//        }
//        break;
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//
//	case WM_LBUTTONDOWN: // $ Левая кнопка нажата
//		//Pause = true;
//		LbuttonClick = true; // $$$$$ Включим режим выделения клеток
//		mousePos.x = xPos;  // $$$$$ Запомним координаты мыши
//		mousePos.y = yPos;
//		calcPoint = grid.GetCell(mousePos); // ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
//		if (calc.Contains(calcPoint, calc.LifePoint)) pointDelete = true; else pointDelete = false; //смотрим есть ли такой элемент
//		mousePosPoint.x = calcPoint.x;
//		mousePosPoint.y = calcPoint.y;
//		calc.Insert(calcPoint, calc.LifePoint, pointDelete);
//		InvalidateRect(hWnd, NULL, false); //перерисовать клиентское окно
//		break;
//
//	case	WM_RBUTTONDOWN: // $ Правая кнопка нажата
//		//Pause = true;
//		DragEnabled = true; // $$$$$ Включим режим таскания сетки
//		mousePos.x = xPos;  // $$$$$ Запомним координаты мыши
//		mousePos.y = yPos;
//
//		break;
//
//	case WM_MOUSEMOVE:  // $$$$$ Мышка двигается 
//		
//		// используем TrackMouseEvent, при выходе за пределы пользовательского окна
//		TRACKMOUSEEVENT tme;
//		tme.cbSize = sizeof(tme);
//		tme.hwndTrack = hWnd;
//		tme.dwFlags = TME_LEAVE;
//		TrackMouseEvent(&tme);
//
//		if (DragEnabled) // $$$$$ При этом зажата правая кнопка т.к. включен режим таскания
//		{
//			// $$$$$ двигаем грид на дельту с прошлого события
//			grid.Move(xPos - mousePos.x, yPos - mousePos.y);
//			mousePos.x = xPos;
//			mousePos.y = yPos;
//			InvalidateRect(hWnd, NULL, false);
//		}
//		else if (LbuttonClick)
//		{
//			//Pause = true;
//			long xPosTmp = xPos - mousePos.x;//разница от текущих координат и записанных в координатах экрана
//			long yPosTmp = yPos - mousePos.y;
//			long MaxPosTmp;// пропуски мышки, которые мы будем обсчитывать
//				
//			if (abs(xPosTmp) > abs(yPosTmp)) MaxPosTmp = abs(xPosTmp); else MaxPosTmp = abs(yPosTmp);//макс пропуск в пикселях
//			//if (MaxPosTmp==0) break; //для одинаковых координат выходим из цикла
//			long ColPosTmp = (MaxPosTmp / grid.scale)+1;//количество циклов расчета координат по этому пропуску с учетом масштаба
//
//			Point start = mousePos;
//
//			for (long i = 0; i < ColPosTmp; i++)
//			{
//				mousePos.x = start.x + (float)xPosTmp/ ColPosTmp*(i+1);
//				mousePos.y = start.y + (float)yPosTmp / ColPosTmp * (i + 1);
//
//				Point calcPoint = grid.GetCell(mousePos); // ИСПОЛЬЗУЕМ НЕ ЭКРАННЫЕ КООРДИНАТЫ, А КООРДИНАТЫ В РАМКАХ КЛИЕНТСКОЙ ОБЛАСТИ ОКНА
//				if (mousePosPoint.x != calcPoint.x || mousePosPoint.y != calcPoint.y)
//				{
//					mousePosPoint.x = calcPoint.x;
//					mousePosPoint.y = calcPoint.y;
//					calc.Insert(calcPoint, calc.LifePoint, pointDelete);
//						
//				}
//			}
//			mousePos.x = xPos;
//			mousePos.y = yPos;
//			InvalidateRect(hWnd, NULL, false);//перерисовать клиентское окно
//
//		}
//
//		break;
//	case WM_MOUSELEAVE: //Сообщение WM_MOUSELEAVE посылается в окно тогда, когда курсор оставляет рабочую область окна, заданную при предшествующем вызове функции TrackMouseEvent.
//		LbuttonClick=false; //отключаем выделение
//		//Pause = false;
//		break;
//
//	case WM_RBUTTONUP: // $$$$$ Правая кнопка отжата
//		DragEnabled = false; // $$$$$ Выключим режим таскания. Если не выключить, сетка будет вечно ходить за мышкой.
//		//Pause = false;
//		break;
//	case WM_LBUTTONUP: // $$$$$ Левая кнопка отжата
//		LbuttonClick = false; // $$$$$ Выключим режим выделения клеток. 
//		//Pause = false;
//		break;
//	case WM_ERASEBKGND://фон окна должен быть стерт (например, когда окно изменено)
//
//		return true;
//
//	case WM_MOUSEWHEEL: // $$$$$ Колесо мышки
//
//		// $$$$$ Тут всякий шлак связанный с тем, что мы можем резко крутануть колесо на пять оборотов, а можем его по чуть чуть крутить. Можно изучить, а можно забить.
//		wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
//		for (; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA) // Вверх
//		{
//			// $$$$$ Увеличиваем масштаб
//
//			grid.AddScale(p.x, p.y);
//			InvalidateRect(hWnd, NULL, false);
//		}
//		for (; wheelDelta < 0; wheelDelta += WHEEL_DELTA) // Вниз
//		{
//			// $$$$$ Уменьшаем масштаб
//
//			grid.DecScale(p.x, p.y);
//			InvalidateRect(hWnd, NULL, false);
//		}
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// Обработчик сообщений для окна "О программе".
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
//




DemoApp::DemoApp() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)
{
}

DemoApp::~DemoApp()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DemoApp::Initialize()
{
	HRESULT hr;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);


		// Create the window.
		m_hwnd = CreateWindow(
			L"D2DDemoApp",
			L"Direct2D Demo App",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
)
{
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			DemoApp app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
			);
		}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
			);
		}
	}

	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp *pDemoApp = (DemoApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
		);

		result = 1;
	}
	else
	{
		DemoApp *pDemoApp = reinterpret_cast<DemoApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pDemoApp->OnRender();
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}
		// Draw two rectangles.
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			rtSize.width / 2 - 50.0f,
			rtSize.height / 2 - 50.0f,
			rtSize.width / 2 + 50.0f,
			rtSize.height / 2 + 50.0f
		);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
			rtSize.width / 2 - 100.0f,
			rtSize.height / 2 - 100.0f,
			rtSize.width / 2 + 100.0f,
			rtSize.height / 2 + 100.0f
		);
		// Draw a filled rectangle.
		m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);
		// Draw the outline of a rectangle.
		m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);
		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	} return hr;
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}