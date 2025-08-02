#include "ToolWindow.h"

bool ToolWindow::Create(HWND parentHwnd, HINSTANCE hInst) {
	hInstance = hInst;

    WNDCLASS wc = {};
    wc.lpfnWndProc = ToolWindow::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ToolWindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0, wc.lpszClassName, NULL,
        WS_CHILD | WS_VISIBLE,
        0, 0,
        200, 50,  // 너비 200으로 줄임, 필요하면 조정
        parentHwnd, nullptr, hInstance, this);

    if (!hwnd) return false;
    
    ShowWindow(hwnd, SW_SHOW);

	return true;
}

LRESULT CALLBACK ToolWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, msg, wParam, lParam);
}