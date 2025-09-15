#include "ToolWindow.h"
#define SAVE 101
#define LOAD 102
#define REPLAY 103
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107

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
    buttonController.Init(hwnd, hInstance);
    CreateButton();
    return true;
}
LRESULT CALLBACK ToolWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   
    ToolWindow* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pcs = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<ToolWindow*>(pcs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hwnd = hwnd;
    }
    else {
        pThis = reinterpret_cast<ToolWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    switch (msg) {
   
    case WM_COMMAND: {
        pThis->buttonController.HandleCommand(wParam);
        return 0;
    }
    }


    return DefWindowProc(hwnd, msg, wParam, lParam);
}



void ToolWindow::CreateButton() {
    buttonController.Create(hwnd, L"저장", SAVE, 10, 10, 80, 30);
    buttonController.Create(hwnd, L"불러오기", LOAD, 130, 10, 80, 30);
    buttonController.Create(hwnd, L"리플레이", REPLAY, 260, 10, 80, 30);
    buttonController.Create(hwnd, L"전체지우기", CLEAR, 390, 10, 80, 30);
    buttonController.Create(hwnd, L"지우기", ERASE, 520, 10, 80, 30);
    buttonController.Create(hwnd, L"브러쉬", BRUSH, 650, 10, 80, 30);
    buttonController.Create(hwnd, L"색상", COLOR, 780, 10, 80, 30);
};


