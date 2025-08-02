#include "MainWindow.h"

bool MainWindow::Create(HINSTANCE hInst, int nCmdShow) {
    hInstance = hInst;

    WNDCLASS wc = {};
    wc.lpfnWndProc = MainWindow::WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MainWindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0, wc.lpszClassName, L"Guest Book", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        900, 700,
        NULL, NULL, hInst, this);

    if (!hwnd) return false;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return true;

	return 0;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void MainWindow::Show(int nCmdShow) {
    if (hwnd) ::ShowWindow(hwnd, nCmdShow);
}



void MainWindow::ResizeChildren() {
    RECT rcClient;
    GetClientRect(hwnd, &rcClient);

    if (toolWindow)
        MoveWindow(toolWindow->GetHwnd(), 0, 0, rcClient.right, 50, TRUE);

    if (drawWindow)
        MoveWindow(drawWindow->GetHwnd(), 0, 50, rcClient.right, rcClient.bottom - 50, TRUE);
}