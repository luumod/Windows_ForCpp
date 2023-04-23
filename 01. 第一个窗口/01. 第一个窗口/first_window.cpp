#include <Windows.h>

//回调函数
LRESULT CALLBACK m_WNDPROC(HWND hwnd, UINT msg, WPARAM wParams, LPARAM lParams) {
	return DefWindowProc(hwnd, msg, wParams, lParams);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cCmdLine,
	int nShowCmd
) {
	// 注册窗口类
	WNDCLASSEX mWinC = {NULL};
	mWinC.cbSize = sizeof(WNDCLASSEX);
	mWinC.hInstance = hInstance;
	mWinC.lpszClassName = "我的第一个窗口";
	mWinC.hbrBackground = (HBRUSH)NULL;
	mWinC.lpfnWndProc = m_WNDPROC;
	mWinC.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&mWinC);
	// 创建窗口类
	HWND hwnd =  CreateWindowEx(mWinC.style,
		mWinC.lpszClassName,
		"First Window",
		WS_OVERLAPPEDWINDOW,
		100, 100, 600, 600,
		NULL, NULL,
		mWinC.hInstance,
		NULL);
	if (!hwnd) return false;
	// 显示窗口类
	ShowWindow(hwnd, SW_SHOW);

	// 刷新窗口类
	UpdateWindow(hwnd);

	// 消息循环
	MSG msg{ NULL };
	while (true) {
		//接收消息
		GetMessage(&msg, NULL, NULL, NULL);
		//处理消息
		TranslateMessage(&msg);
		//发送消息
		DispatchMessage(&msg);
	}

	// 消息处理函数

	return true;
}