#include "System/Window.h"
#include "System/Logger.h"

#if PLATFORM_WINDOWS

#include <Windows.h>
#include <stdlib.h>

static const char* WindowClassName = "RandomWindowClassName";
static u64 s_WindowCount = 0;

#include "System/Private/Win32WindowData.inl"

static LRESULT CALLBACK Win32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* window = (Window*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

	switch (uMsg)
	{
	case WM_SIZE:
		if (window->ResizeCallback)
			window->ResizeCallback(window, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_QUIT:
	case WM_CLOSE:
		if (window->CloseCallback)
			window->CloseCallback(window);
		return 0;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

Window* WindowCreate(u32 width, u32 height, const char* title)
{
	Window* window = (Window*)malloc(sizeof(Window) + sizeof(Win32WindowData));
	window->Data			= NULL;
	window->CloseCallback	= NULL;
	window->ResizeCallback	= NULL;
	window->Data = ((char*)window) + sizeof(Window);

	Win32WindowData* data = (Win32WindowData*)window->Data;
	data->HInstance = GetModuleHandle(NULL);
	data->HWnd = NULL;

	if (s_WindowCount == 0)
	{
		WNDCLASSEXA wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.lpszClassName = WindowClassName;
		wc.lpfnWndProc = Win32WindowProc;
		wc.hInstance = data->HInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClassExA(&wc);
	}

	DWORD style =
		WS_OVERLAPPED	|
		WS_CAPTION		|
		WS_SYSMENU		|
		WS_THICKFRAME	|
		WS_MINIMIZEBOX	|
		WS_MAXIMIZEBOX;

	RECT wr = { 0 };
	wr.left = 100;
	wr.right = wr.left + width;
	wr.top = 100;
	wr.bottom = wr.top + height;

	AdjustWindowRect(&wr, style, 0);

	data->HWnd = CreateWindowExA(
		0,
		WindowClassName,
		title,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		data->HInstance,
		NULL
	);
	s_WindowCount++;

	SetWindowLongPtrA(data->HWnd, GWLP_USERDATA, (LONG_PTR)window);

	LOG_INFO("Created Window '%s': %d, %d", title, width, height);
	return window;
}

void WindowDestroy(Window* window)
{
	Win32WindowData* data = (Win32WindowData*)window->Data;

	DestroyWindow(data->HWnd);
	s_WindowCount--;
	if (s_WindowCount == 0)
	{
		UnregisterClassA(WindowClassName, data->HInstance);
	}
	free(window);
}

void WindowShow(Window* window)
{
	Win32WindowData* data = (Win32WindowData*)window->Data;
	ShowWindow(data->HWnd, SW_SHOW);
}

void WindowHide(Window* window)
{
	Win32WindowData* data = (Win32WindowData*)window->Data;
	ShowWindow(data->HWnd, SW_HIDE);
}

void WindowUpdate(Window* window)
{
	Win32WindowData* data = (Win32WindowData*)window->Data;

	static MSG msg;
	while (PeekMessageA(&msg, data->HWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

#else
	#error "Implement window for this platform!"
#endif
