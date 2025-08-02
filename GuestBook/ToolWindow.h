#pragma once
#include <windows.h>

class DrawWindow;

class ToolWindow
{
public:
    ToolWindow() = default;
    bool Create(HWND parentHwnd, HINSTANCE hInstance);
    HWND GetHwnd() const { return hwnd; }

    void SetDrawWindow(DrawWindow* draw) { drawWindow = draw; }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hwnd = nullptr;
    HWND replayHwnd = nullptr;
    HINSTANCE hInstance = nullptr;

    DrawWindow* drawWindow = nullptr;
};

