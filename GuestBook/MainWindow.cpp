#include "MainWindow.h"
#include "DrawWindow.h"
#include "resource.h"
#include "Sidebar.h"
bool MainWindow::Create(HINSTANCE hInst, int nCmdShow) {
    hInstance = hInst;

    WNDCLASS wc = {};
    wc.lpfnWndProc = MainWindow::WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MainWindowClass";
    wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(220,220,220)));
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOGO));
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
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = nullptr;
   
    switch (msg) {
    case WM_NCCREATE: {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<MainWindow*>(cs->lpCreateParams);
        if (pThis) {

            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->hwnd = hwnd;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    }

    pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (pThis) return pThis->HandleMessage(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {
        OutputDebugString(L"Main WM_CREATE called\n");
    case WM_CREATE: {
        sideBar.SetSlider(hwnd, hInstance);
        return 0;

    }
    case WM_SIZE: {
        ResizeChildren();
        return 0;
    }
    case WM_VSCROLL: {
        HWND hSlider = (HWND)lParam; // 메시지를 보낸 컨트롤(슬라이더)의 핸들

        if (hSlider == sideBar.GetSliderHandle()) {

            if (LOWORD(wParam) == TB_THUMBTRACK || LOWORD(wParam) == TB_ENDTRACK) {

                int newPenWidth = SendMessage(hSlider, TBM_GETPOS, 0, 0);

                drawWindow->SetPenWidth(newPenWidth);
            }
        }
        return 0;
    }
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

    if (toolWindow){
        MoveWindow(toolWindow->GetHwnd(),
            rcClient.left, rcClient.top,
            rcClient.right, 50,
            TRUE);
    }

    if (drawWindow) {
        MoveWindow(drawWindow->GetHwnd(),
            rcClient.left + 100, rcClient.top + 60,
            rcClient.right-110, rcClient.bottom - 70,
            TRUE);
    }

    if (sideBar.GetSliderHandle()) {
        MoveWindow(sideBar.GetSliderHandle(),
            25, rcClient.top + 120,     // X, Y
            50, rcClient.bottom - 200, // 너비, 높이
            TRUE);
    }

    int w = rcClient.right - rcClient.left;
    int h = rcClient.bottom - rcClient.top;

    if (w > 0 && h > 0) {
        HDC hdc = GetDC(hwnd);
        backBuffer.CreateBuffer(hdc, w, h);
        ReleaseDC(hwnd, hdc);

        backBuffer.ClearBuffer(rcClient);

        if (drawWindow)
            drawWindow->setBuffer(backBuffer);
    }
    InvalidateRect(hwnd, nullptr, FALSE); 
}