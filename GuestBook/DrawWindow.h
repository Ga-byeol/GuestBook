#pragma once
#include <windows.h>

class ToolWindow;

class DrawWindow
{
public:
    DrawWindow() = default;
    bool Create(HWND parentHwnd, HINSTANCE hInstance);
    HWND GetHwnd() const { return hwnd; }

    void SetToolWindow(ToolWindow* tool) { toolWindow = tool; }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hwnd = nullptr;
    HINSTANCE hInstance = nullptr;

    ToolWindow* toolWindow = nullptr;
};

