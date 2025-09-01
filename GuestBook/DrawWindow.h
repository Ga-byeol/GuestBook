#pragma once
#include <windows.h>
#include <vector>
#include "Stroke.h"
#include "UIDrawBridge.h"
#include "DrawController.h"
#include "StrokeStore.h"

class ToolWindow;
class UIDrawBridge;

class DrawWindow
{
public:
    DrawWindow() = default;
    bool Create(HWND parentHwnd, HINSTANCE hInstance);
    HWND GetHwnd() const { return hwnd; }

    void SetToolWindow(ToolWindow* tool) { toolWindow = tool; }

    /// 주입하는 통로
    void SetBridge(UIDrawBridge* b) { bridge = b; }

    /// 저장소 포인터를 제공
    StrokeStore* GetStore() { return &store; }

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

    /// 보관
    UIDrawBridge* bridge = nullptr;
};