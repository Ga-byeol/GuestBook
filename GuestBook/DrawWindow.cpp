#include "DrawWindow.h"

bool DrawWindow::Create(HWND parentHwnd, HINSTANCE hInst) {
    hInstance = hInst;

    WNDCLASS wc = {};
    wc.lpfnWndProc = DrawWindow::WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"DrawWindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0, wc.lpszClassName, L"Guest Book", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 700,
        NULL, NULL, hInst, this);

    if (!hwnd) return false;

	return true;
}

LRESULT CALLBACK DrawWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, msg, wParam, lParam);
}