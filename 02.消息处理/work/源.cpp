#include <Windows.h>
#include <cstdio>

HANDLE console;

//消息回调
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_TIMER:
	{
		int wmID = LOWORD(wParam);
		auto choi = MessageBox(hwnd, L"强哥帅不帅?", L"警告!", MB_YESNO);
		if (choi == IDYES) {
			PostQuitMessage(0);
		}
		break;
	}
	case WM_CREATE:{
		SetTimer(hwnd, 1234, 1000, NULL);
		break;
	}
	case WM_DESTROY:
		while (MessageBox(hwnd, L"想退出？先回答我强哥帅不帅!", L"提示", MB_YESNO)!=IDYES) {}
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPreInstace,
	LPSTR nCmaLine,
	int cmdShow
) {
	AllocConsole();
	console = GetStdHandle(STD_OUTPUT_HANDLE);

	//注册窗口类
	WNDCLASSEXW wcex{ NULL };

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
	wcex.lpszClassName = L"强哥牛逼";
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);
	//创建窗口
	HWND hwnd = CreateWindowExW(
		wcex.style,
		wcex.lpszClassName,
		L"强哥666",
		WS_OVERLAPPEDWINDOW, //窗口风格
		100, 100, 500, 500,
		NULL, NULL, wcex.hInstance, NULL
	);
	//显示窗口
	ShowWindow(hwnd, SW_SHOW);
	//刷新窗口
	UpdateWindow(hwnd);
	
	MSG msg{};
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	CloseWindow(hwnd);
	return 0;
}