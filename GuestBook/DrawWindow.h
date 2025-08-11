#pragma once
#include <windows.h>
#include <vector>
#include "StructStroke.h"
#include "DrawController.h"
#include "StrokeStore.h"

class ToolWindow;

class DrawWindow
{
public:
    DrawWindow() = default;
    bool Create(HWND parentHwnd, HINSTANCE hInstance);
    HWND GetHwnd() const { return hwnd; }

    void SetToolWindow(ToolWindow* tool) { toolWindow = tool; }

private:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void OnPaint(HDC hdc, const RECT& rc);
    void OnLButtonDown(int x, int y, WPARAM flags);
    void OnMouseMove(int x, int y, WPARAM flags);
    void OnLButtonUp(int x, int y, WPARAM flags);

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hwnd = nullptr;
    HINSTANCE hInstance = nullptr;

    ToolWindow* toolWindow = nullptr;

    DrawController controller;
    StrokeStore    store;
};
