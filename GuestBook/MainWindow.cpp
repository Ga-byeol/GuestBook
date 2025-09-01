#include "MainWindow.h"
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107

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
        0, wc.lpszClassName, L"Guest Book", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        900, 700,
        NULL, NULL, hInst, this);

    if (!hwnd) return false;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return true;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    /// GetWindowLongPtr(hwnd, GWLP_USERDATA)으로 hwnd창의 사용자 저장칸 안에 전에 넣어둔
    /// this 포인터 값을 꺼내 MainWindow* 타입 포인터로 가져옴
    
    /// self(HWND 담당하는 this의 포인터)를 멤버(self->ResizeChildren()로 사용함
    MainWindow* self = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    
    if (msg == WM_NCCREATE) {
        /// CreatWindowEx에서 보낸 this가 WM_NCCREATE lParam에 들어옴
        /// GWLP_USERDATA에 1번 저장 후 GetWindowLongPtr로 꺼내면 됨

        auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = reinterpret_cast<MainWindow*>(cs->lpCreateParams);
        if (self) {
            self->hwnd = hwnd;

            /// Win32 API함수 hwnd 윈도우 저장칸에 객체 주소인 self 넣음
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
        }
        return TRUE;
    }

    /// OS 제공 기본 처리기(처리 안 하면 처리 넘기기)
    if (!self) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg) {

    case WM_SIZE:
    {
        if (self) {
           /// self->ConnectUIDraw();
            self->ResizeChildren();
            /// 자식창 클라이언트 크기 맞추어 재배치

            if (self->drawWindow) {

                ///백버퍼 크기 맞추기 위해 Draw창 핸들Hwnd 얻기
                HWND drawHwnd = self->drawWindow->GetHwnd();
                RECT rc;
                GetClientRect(drawHwnd, &rc);   /// 드로우 클라이언트 크기 얻음
                int w = rc.right - rc.left;
                int h = rc.bottom - rc.top;
                if (w > 0 && h > 0) {
                    HDC drawDc = GetDC(drawHwnd);     
                    self->backbuffer.CreateBuffer(drawDc, w, h);    ///drawDc기준으로 버퍼 생성         
                    self->frontbuffer.CreateBuffer(drawDc, w, h);                          

                    ///만든 버퍼를 즉시 초기화하여 깜빡임을 방지하기
                    RECT r{ 0,0,w,h };
                    self->backbuffer.ClearBuffer(r);
                    self->frontbuffer.ClearBuffer(r);
                    ReleaseDC(drawHwnd, drawDc);    /// DC 반납  

                    SendMessage(drawHwnd, WM_APP + 1, 0, 0);
                    InvalidateRect(drawHwnd, nullptr, FALSE);    
                }
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

    if (toolWindow)
        MoveWindow(toolWindow->GetHwnd(), 0, 0, rcClient.right, 50, TRUE);

    if (drawWindow)
        MoveWindow(drawWindow->GetHwnd(), 0, 50, rcClient.right, rcClient.bottom - 50, TRUE);
}

void MainWindow::ConnectUIDraw() {
    if (!drawWindow || !toolWindow) return;
    if (!drawWindow->GetHwnd() || !toolWindow->GetHwnd()) return;

    bridge.store = drawWindow->GetStore();
    bridge.hDraw = drawWindow->GetHwnd();

    drawWindow->SetBridge(&bridge);  // Draw: 게터로 읽기만
    SetProp(toolWindow->GetHwnd(), L"APP_BRIDGE", &bridge);
}