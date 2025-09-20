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
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
   
    switch (msg) {
    case WM_NCCREATE: {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<MainWindow*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->hwnd = hwnd;
        return TRUE;
    }

    case WM_CREATE: {
        RECT rc;
        GetClientRect(hwnd, &rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;

        HDC hdc = GetDC(hwnd);
        pThis->back.CreateBuffer(hdc, w, h);
        ReleaseDC(hwnd, hdc);
        return 0;
    }

    case WM_SIZE: {
        int w = LOWORD(lParam);
        int h = HIWORD(lParam);

<<<<<<< HEAD
            HDC hdc = GetDC(hwnd);
<<<<<<< HEAD
            BackBufferManager::Instance().ResizeBuffer(hdc, rc);
=======
            BackBufferManager::Instance().ResizeBuffer(hdc, clientWidth, clientHeight);
>>>>>>> eee0e96 (Feature/replaycontroller (#18))
            ReleaseDC(hwnd, hdc); /// hdc �ݳ�
=======
        HDC hdc = GetDC(hwnd);
        pThis->back.CreateBuffer(hdc, w, h);
        ReleaseDC(hwnd, hdc); /// hdc �ݳ�
>>>>>>> e953b4c (feat: BackBuffer::DrawDirtyBufferToScreen 구현, MainWindow::WM_CREATE/WM_SIZE 시점 버퍼 생성 및 재생성 처리 구현, DrawWindow 실시간 그리기 구현 (진행 중))

        pThis->ResizeChildren();

        InvalidateRect(hwnd, nullptr, TRUE);
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

    if (toolWindow)
        MoveWindow(toolWindow->GetHwnd(), 0, 0, rcClient.right, 50, TRUE);

    if (drawWindow)
        MoveWindow(drawWindow->GetHwnd(), 0, 50, rcClient.right, rcClient.bottom - 50, TRUE);
}