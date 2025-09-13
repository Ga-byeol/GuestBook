#pragma once
#include <windows.h>
#include <vector>
#include <condition_variable>
#include "Stroke.h"
#include "DrawController.h"
#include "StrokeStore.h"

#define WM_USER_REPLAY_UPDATE (WM_USER + 1)

class ToolWindow;

class DrawWindow
{
public:
    bool Create(HWND parentHwnd, HINSTANCE hInstance);
    HWND GetHwnd() const { return hwnd; }
    StrokeStore& GetStore() { return store; }


    void SetToolWindow(ToolWindow* tool) { toolWindow = tool; }

    void setSelectedColor(COLORREF color) { this->selectedColor = color; }
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
    StrokeStore store;
    COLORREF selectedColor;
};