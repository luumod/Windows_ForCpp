// 05.GDI绘图.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "05.GDI绘图.h"
#include <wingdi.h>
#pragma comment(lib,"Msimg32.lib")
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
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

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY05GDI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY05GDI));

	MSG msg;

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY05GDI));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY05GDI);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//1. 普通绘图
void draw(HWND hwnd) {
	//获取HDC
	HDC hdc = GetDC(hwnd);
#if 0
	//绘制像素点
	for (int i = 1; i <= 100; i++) {
		SetPixel(hdc, 100 + i, 100, RGB(255, 0, 0));
	}
		
	POINT oldPos{};
	MoveToEx(hdc, 200, 100, NULL);
	LineTo(hdc, 300, 300);
	//保存（300,300）的位置，并且移动到（100，100）的位置
	MoveToEx(hdc, 100, 100, &oldPos);  
	//画线到（300，300）的位置
	LineTo(hdc, oldPos.x, oldPos.y);  
	//形成一个三角形
#elif 0
	//1.自制画笔
	HPEN pSolidPen1 =  CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HPEN pDashPen1 = CreatePen(PS_DASH, 5, RGB(0, 255, 0));

	//2.画笔交给画家
	HGDIOBJ hOldObj = SelectObject(hdc, pSolidPen1);
	MoveToEx(hdc,100, 100, NULL);
	LineTo(hdc, 200, 200);  //使用pSolidPen1来绘制

	hOldObj = SelectObject(hdc, pDashPen1); //pOldObj保存pSolidPen1
	LineTo(hdc, 300, 300); //使用pDashPen1来绘制

	SelectObject(hdc, hOldObj); //使用pSolidPen1来绘制一个矩形
	Rectangle(hdc, 100,200,150,300);
	//Arc(hdc, 100, 100, 50, 50, 150, 150, 200, 200);
#elif 0
	//1.自制画刷
	HBRUSH hBrush1 = CreateSolidBrush(RGB(30, 30, 30));
	HBRUSH hBrush2 = CreateSolidBrush(RGB(20, 210, 20));

	//2.画刷给画家
	SelectObject(hdc, hBrush1);
	Rectangle(hdc, 10, 10, 100, 100);

	SelectObject(hdc, hBrush2);
	Ellipse(hdc, 200, 200, 400, 400);
#endif
}

//2. 贴图
void putImg(HWND hwnd) {
	HDC hdc =  GetDC(hwnd);
	HINSTANCE hInstanc =  GetModuleHandle(NULL);

	//1. 加载图片
	HANDLE bk = LoadImage(hInstanc, L"bk.bmp", IMAGE_BITMAP, 1400, 770, LR_LOADFROMFILE);
	HANDLE p1 =  LoadImage(hInstanc, L"img1.bmp", IMAGE_BITMAP, 207, 293, LR_LOADFROMFILE);
	HANDLE p2 =  LoadImage(hInstanc, L"img2.bmp", IMAGE_BITMAP, 299, 286, LR_LOADFROMFILE);
	HANDLE p3 =  LoadImage(hInstanc, L"img3.bmp", IMAGE_BITMAP, 265, 330, LR_LOADFROMFILE);

#if 0
	//2. 创建兼容DC
	HDC hBk = CreateCompatibleDC(NULL);
	HDC hDc1 = CreateCompatibleDC(NULL);//与指定设备兼容的DC
	HDC hDc2 = CreateCompatibleDC(NULL);
	HDC hDc3 = CreateCompatibleDC(NULL);
	//3. 把图片选择到兼容DC
	SelectObject(hBk, bk);
	SelectObject(hDc1, p1); //将对象选择到按指定的设备上下文中
	SelectObject(hDc2, p2);
	SelectObject(hDc3, p3);
	//4. 从兼容DC绘制到HDC
	BitBlt(hdc, 0, 0, 1400, 770, hBk, 10,10, SRCCOPY);
	BitBlt(hdc, 10, 10, 207, 293, hDc1, 0, 0, SRCCOPY);
	BitBlt(hdc, 300, 10, 299, 286, hDc2, 0, 0, SRCCOPY);
	BitBlt(hdc, 600, 10, 265, 330, hDc3, 0, 0, SRCCOPY);
#else
	//2. 创建兼容位图
	HBITMAP hBit1 = CreateCompatibleBitmap(hdc, 1500, 770);
	//3. 创建兼容DC
	HDC hBk = CreateCompatibleDC(NULL);
	HDC hDc1 =  CreateCompatibleDC(NULL);
	HDC hDc2 = CreateCompatibleDC(NULL);
	HDC hDc3 = CreateCompatibleDC(NULL);
	HDC hDst = CreateCompatibleDC(NULL); //最终的画布
	//4. 兼容位图设置到兼容DC中
	SelectObject(hDst, hBit1);
	//5. 图片选择到兼容DC中
	SelectObject(hBk, bk);
	SelectObject(hDc1, p1);
	SelectObject(hDc2, p2);
	SelectObject(hDc3, p3);
	//6. 每个兼容DC绘制到总的兼容DC中（总画布）
	BitBlt(hDst, 0, 0, 1500, 770, hBk, 10, 10, SRCCOPY);
	BitBlt(hDst, 0, 0, 207, 293, hDc1, 0, 0, SRCCOPY);
	BitBlt(hDst, 250,10, 299, 286, hDc2, 0, 0, SRCCOPY);
	BitBlt(hDst, 600, 10, 265, 330, hDc3, 0, 0, SRCCOPY);
	//7. 最终兼容DC绘制到HDC
	TransparentBlt(hdc, 0, 0,
		GetSystemMetrics(SM_CXFULLSCREEN),
		GetSystemMetrics(SM_CYFULLSCREEN),
		hDst, 0, 0, 1500, 770, SRCCOPY);
	ReleaseDC(hwnd, hdc);
#endif
}


void text(HWND hwnd) {
	HDC hdc = GetDC(hwnd);
	HFONT hFont1 = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	SelectObject(hdc, hFont1);
	wchar_t buff[256] = L"这是一段文字";
	TextOut(hdc, 100, 100, buff, wcslen(buff));

	ReleaseDC(hwnd,hdc);
}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			
			//draw(hWnd);
			//putImg(hWnd);
			text(hWnd);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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
