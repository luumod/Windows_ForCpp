// 04.菜单消息.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "04.菜单消息.h"
#include <cstdio>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

//控制台
HANDLE console;

//全局菜单
HMENU g_sysMenu;
HMENU g_popMenu;
HMENU g_htop;

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
	LoadStringW(hInstance, IDC_MY04, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY04));

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
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY04));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY04);
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

//窗口创建时
void onCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	
	//开启控制台
	AllocConsole();
	console = GetStdHandle(STD_OUTPUT_HANDLE);

	//获得系统菜单
	g_sysMenu = GetSystemMenu(hwnd, false);
	//增加菜单项
	//DeleteMenu(g_sysMenu, 4, MF_BYPOSITION);
	AppendMenu(g_sysMenu, MF_STRING, 0x0001, L"新增菜单(&o)");


	//二：右键菜单
	g_popMenu = CreatePopupMenu();
	AppendMenu(g_popMenu,MF_STRING,0x0002,L"打开(&1)");
	AppendMenu(g_popMenu, MF_STRING, 0x0003, L"复制(&2)");
	AppendMenu(g_popMenu, MF_SEPARATOR, 0x0004, NULL);
	AppendMenu(g_popMenu, MF_STRING, 0x0005, L"粘贴");
	AppendMenu(g_popMenu, MF_STRING, 0x0006, L"剪切");
	


	//三：创建菜单栏
	HMENU hTop = CreateMenu();

	//将弹出式菜单作为顶层菜单的子菜单
	HMENU p1 = CreatePopupMenu();
	HMENU p2 = CreatePopupMenu();
	HMENU p3 = CreatePopupMenu();
	HMENU p4 = CreatePopupMenu();

	AppendMenu(hTop, MF_POPUP, (UINT_PTR)p1, L"编辑");
	AppendMenu(hTop, MF_POPUP, (UINT_PTR)p2, L"视图");
	AppendMenu(hTop, MF_POPUP, (UINT_PTR)p3, L"Git");
	AppendMenu(hTop, MF_POPUP, (UINT_PTR)p4, L"项目");

	//弹出式菜单增加子菜单栏
	AppendMenu(p1, MF_POPUP, 0x0010, L"新建");
	AppendMenu(p1, MF_POPUP, 0x0011, L"打开");
	AppendMenu(p1, MF_POPUP, 0x0012, L"克隆存储库");
	AppendMenu(p1, MF_SEPARATOR, 0x0013, NULL);
	AppendMenu(p1, MF_STRING, 0x0014, L"启动");

	//弹出式菜单的子菜单栏再次增加子菜单
	HMENU pp1 = CreatePopupMenu();
	AppendMenu(pp1, MF_POPUP, 0x0015, L"项目");
	AppendMenu(pp1, MF_POPUP, 0x0016, L"存储库");
	AppendMenu(pp1, MF_POPUP, 0x0017, L"文件");
	AppendMenu(pp1, MF_SEPARATOR, 0x0018, NULL);
	AppendMenu(pp1, MF_STRING, 0x0019, L"返回");

	AppendMenu(p1, MF_POPUP, (UINT_PTR)pp1, L"添加");

	//设置顶层菜单显示
	SetMenu(hwnd, hTop);
}

//右键菜单的位置设定
//获取窗口坐标
void onRButtonUpMenu(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	int x = LOWORD(lParam), y = HIWORD(lParam);
	POINT p{ x,y };
	//需要将窗口坐标转换为屏幕坐标
	ClientToScreen(hwnd, &p);


	RECT menuRect{}, winRect{};
	GetWindowRect(hwnd, &winRect);

	//获取菜单项的矩形区域
	GetMenuItemRect(hwnd, g_popMenu, NULL, &menuRect);

	UINT fg = TPM_LEFTALIGN | TPM_TOPALIGN;//默认左弹出

	//超过了窗口右边界，则左边显示
	if (p.x + menuRect.right-menuRect.left > winRect.right - winRect.left) {
		fg = TPM_RIGHTALIGN | TPM_TOPALIGN;
	}
	//如果超过了窗口的下边界，则上边显示
	if (p.y + menuRect.bottom - menuRect.top > winRect.bottom - winRect.top) {
		fg = TPM_BOTTOMALIGN;
	}
	//如果既超过了右，又超过了下，则左上显示
	if (p.x + menuRect.right - menuRect.left > winRect.right - winRect.left &&
		p.y + menuRect.bottom - menuRect.top > winRect.bottom - winRect.top) {
		fg = TPM_RIGHTALIGN | TPM_BOTTOMALIGN;
	}
	TrackPopupMenu(g_popMenu, fg, p.x, p.y, NULL, hwnd, NULL);

#if 0
	wchar_t buff[256]{};
	wsprintf(buff, L"width: %d ;height: %d\n", menuRect.right-menuRect.left,menuRect.bottom-menuRect.top);
	WriteConsole(console, buff, wcslen(buff), NULL, NULL);
#endif
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
	case WM_CREATE:
		onCreate(hWnd,wParam,lParam);
		break;
	case WM_RBUTTONUP: {
		onRButtonUpMenu(hWnd, wParam, lParam);
		break;
	}
	case WM_SYSCOMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case 0x0001:
			MessageBox(NULL, L"弹出了一个窗口!", L"提示", NULL);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
			case 0x0002:
				MessageBox(NULL, L"打开", L"提示", NULL);
				break;
			case 0x0003:
				MessageBox(NULL, L"复制", L"提示", NULL);
				break;
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
