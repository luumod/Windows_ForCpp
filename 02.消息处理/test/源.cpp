#include <Windows.h>
#include <cstdio>

HANDLE console; //控制台

void CALLBACK TimeProc(HWND hwnd, UINT uint , UINT_PTR uPtr, DWORD dword) {
	//MessageBox(hwnd, "警告!", "Warning", NULL);
	//中断消息
}

//消息处理函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	//获取时间
	SYSTEMTIME systime{};

	static int n = 0;
	char buff[256]{};
	sprintf(buff, "%d: 消息: %d \n", n++, message);
	WriteConsole(console, buff, strlen(buff), NULL, NULL);

	
	switch (message)
	{
	case WM_TIMER: {
		GetLocalTime(&systime);
		int wmID = LOWORD(wParam);  //获取定时器的消息
		char timebuff[256]{};
		if (wmID == 11111) {
			sprintf(timebuff, "1111消息: 定时器消息: %d-%d-%d\n", systime.wYear, systime.wMonth, systime.wDay);
			
		}
		else {
			sprintf(timebuff, "other消息: 定时器消息: %d-%d-%d\n", systime.wYear, systime.wMonth, systime.wDay);
		}
		WriteConsole(console, timebuff, strlen(timebuff), NULL, NULL);
		break;
	}
	case WM_CREATE:
		//在创建窗口之前显示此消息
		MessageBox(hwnd, "我创建了", "ylh的Box\n", NULL);
		//中断消息
		SetTimer(hwnd, 11111, 222, NULL); //第四个参数是NULL，则发送消息
		SetTimer(hwnd, 22222, 333, NULL);
		//轮询消息
		SetTimer(hwnd, 33333, 444, (TIMERPROC)TimeProc);
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(666);//发送WM_QUIT消息，导致GetMessage函数返回0
		char buff[256]{};
		sprintf(buff, "退出消息: %d\n", WM_QUIT);
		WriteConsole(console, buff, strlen(buff), NULL, NULL);
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
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