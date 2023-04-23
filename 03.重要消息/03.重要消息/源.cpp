#include <Windows.h>
#include <cstdio>
#include <Dbt.h>
#include "resource.h"

HANDLE console; //控制台

//定时器中断函数
void CALLBACK TimeProc(HWND hwnd, UINT uint , UINT_PTR uPtr, DWORD dword) {
	//中断消息
}
//窗口创建消息
void onCreate() {
	//打开控制台
	AllocConsole();
	//设置标准输出设备
	console = GetStdHandle(STD_OUTPUT_HANDLE);
}
//定时器消息
void onTimer() {
}
//窗口销毁消息
void onDestroy() {
	PostQuitMessage(666);//发送WM_QUIT消息，导致GetMessage函数返回0
}
//鼠标移动消息
void onMouseMove(WPARAM wParam, LPARAM lParam) {
	if (wParam == MK_CONTROL) {
		char buff[256]{};
		sprintf(buff, "按下了Control键。\n");
		WriteConsole(console, buff, strlen(buff), NULL, NULL);
	}
	int x = LOWORD(lParam), y = HIWORD(lParam);
	char buff[256]{};
	sprintf(buff, "(x,y) = (%d,%d)\n", x, y);
	WriteConsole(console, buff, strlen(buff), NULL, NULL);
}
//鼠标左键按下消息
void OnMouseClick(WPARAM wParam,LPARAM lParam) {
	int x = LOWORD(lParam), y = HIWORD(lParam);
	char buff[256]{};
	sprintf(buff, "(x,y) = (%d,%d)\n", x, y);
	WriteConsole(console, buff, strlen(buff), NULL, NULL);
}
//鼠标滑轮消息
void onMouseWheel(WPARAM wParam, LPARAM lParam) {
	int x = LOWORD(lParam), y = HIWORD(lParam);
	char buff[256]{};
	//正表示往前；负值表示往后
	sprintf(buff, "wParam: %d\n", (int)wParam / WHEEL_DELTA);
	WriteConsole(console, buff, strlen(buff), NULL, NULL);
}
//案例：鼠标样式切换
void MouseChange(HWND hwnd,WPARAM wParam,LPARAM lParam) {
	HINSTANCE hInstance =  GetModuleHandle(NULL);
	HCURSOR cursor1 = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	HCURSOR cursor2 = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR2));
	int x = LOWORD(lParam), y = HIWORD(lParam);
	RECT rect;
	GetWindowRect(hwnd, &rect);
	if (x < rect.right - rect.left >> 1) {
		SetCursor(cursor1);
	}
	else {
		SetCursor(cursor2);
	}
}
//按键消息
void onKeyDown(WPARAM wParam, LPARAM lParam) {
	char buff[256]{};
	//wParam表示虚拟按键代码
	int f = LOWORD(lParam);
	sprintf(buff, "键盘按下: %d\n", (int)wParam);
	WriteConsole(console, buff, strlen(buff), NULL, NULL);
}
//按键弹起
void onKeyUp(WPARAM wParam, LPARAM lParam) {
	char buff[256]{};
	//wParam表示虚拟按键代码
	sprintf(buff, "wParam键盘弹起: %d\n", (int)wParam);
	WriteConsole(console, buff, strlen(buff), NULL, NULL);
}
//外部设备消息（引入: #include <Dbt.h>）
void onDevice(WPARAM wParam,LPARAM lParam) {
	char buff[256]{};
	//wParam表示虚拟按键代码
	//插入设备： 0x8000
	if (wParam == DBT_DEVICEARRIVAL) {
		auto pDev = (PDEV_BROADCAST_HDR)lParam;
		auto pVol = (PDEV_BROADCAST_VOLUME)pDev;

		DWORD dw = pVol->dbcv_unitmask; //存储了盘符的32位数据
		auto f = [&]() {
			int i = 0;
			for (; i < 26; i++) {
				if (dw & 1) {
					break;
				}
				dw >>= 1; //右移一位
			}
			return i + 'A';//获取盘符
		};
		char panfu = f();
		sprintf(buff, "U盘的盘符是: %c\n", panfu);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "xcopy %c:\\test E:\\dst /E", panfu);
		system(buff);
	}
}
//消息处理函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message)
	{
	case WM_LBUTTONDBLCLK: {
		char buff[256]{};
		sprintf(buff, "鼠标双击了");
		WriteConsole(console, buff, strlen(buff), NULL, NULL);
		break;
	}
	case WM_DEVICECHANGE:
		onDevice(wParam, lParam);
		break;
	case WM_KEYUP:
		//onKeyUp(wParam, lParam);
		break;
	case WM_KEYDOWN:
		//onKeyDown(wParam,lParam);
		break;
	case WM_MOUSEWHEEL:
		onMouseWheel(wParam, lParam);
		break;
	case WM_MBUTTONDOWN://鼠标中键按下
	case WM_RBUTTONDOWN://鼠标右键按下
	case WM_LBUTTONDOWN://鼠标左键按下
		OnMouseClick(wParam,lParam);
		break;
	case WM_MOUSEMOVE:
		onMouseMove(wParam,lParam);
		//鼠标移动时样式切换
		MouseChange(hwnd, wParam, lParam);
		break;
	case WM_TIMER: {
		onTimer();
		break;
	}
	case WM_CREATE:
		onCreate();
		break;
	case WM_DESTROY:
	{
		onDestroy();
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmd,
	int flag
) {
	//调用控制台
	AllocConsole();
	console = GetStdHandle(STD_OUTPUT_HANDLE);

	//注册窗口类
	WNDCLASSEXW wcex{NULL};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc; //消息处理函数
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;  //图标
	wcex.hCursor = NULL; //光标
	wcex.hbrBackground = NULL;//画刷
	wcex.lpszMenuName = NULL;//菜单名字
	wcex.lpszClassName = L"ylh的窗口类";
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);
	//创建窗口
	HWND hwnd =  CreateWindowExW(
		wcex.style,
		wcex.lpszClassName,
		L"ylh牛逼",
		WS_OVERLAPPEDWINDOW, //窗口风格
		100, 100, 500, 500,
		NULL, NULL, wcex.hInstance, NULL
	);
	//显示窗口
	ShowWindow(hwnd, SW_SHOW);
	//刷新窗口
	UpdateWindow(hwnd);
	//消息循环
	MSG msg{};
	while (GetMessage(&msg, NULL, NULL, NULL)) {  //GetMessage的返回值控制退出
		//翻译消息
		TranslateMessage(&msg);
		//发送消息
		DispatchMessage(&msg);
	}
	system("pause");
	return 0;
}