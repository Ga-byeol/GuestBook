#include "Application.h"

bool Application::Init(HINSTANCE hInst, int nCmdShow) {

    hInstance = hInst;

    if (!mainWindow.Create(hInstance, nCmdShow))
        return false;

    if (!drawWindow.Create(mainWindow.GetHwnd(), hInstance))
        return false;

    if (!toolWindow.Create(mainWindow.GetHwnd(), hInstance))
        return false;

    // ���� ����
    mainWindow.SetDrawWindow(&drawWindow);
    mainWindow.SetToolWindow(&toolWindow);

    toolWindow.SetDrawWindow(&drawWindow);
    drawWindow.SetToolWindow(&toolWindow);

    mainWindow.ResizeChildren();
    mainWindow.Show(nCmdShow);

    return true;
}

int Application::Run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

void Application::DrawForReplay() {
    OutputDebugString(L"draw\n");
    PostMessage(drawWindow.GetHwnd(), WM_USER_REPLAY_UPDATE, 0, 0);
}
