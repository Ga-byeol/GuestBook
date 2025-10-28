#pragma once
#include <windows.h>
#include <vector>
#include <condition_variable>
#include "Stroke.h"
#include "DrawController.h"
#include "StrokeController.h"
#include "StrokeStore.h"
#include "BackBuffer.h"
#include "BackBufferManager.h"

#define WM_USER_REPLAY_UPDATE (WM_USER + 1)

class ToolWindow;
class MainWindow;
class BackBuffer;
class Application;

class DrawWindow
{
public:
    bool Create(HWND parentHwnd, HINSTANCE hInstance);
    HWND GetHwnd() const { return hwnd; }
    HDC GetMemDc() const;
    StrokeStore& GetStore() { return store; }
    const std::vector<Stroke>& GetDrawnStrokes() const { return strokeCtrl.Strokes(); }

    void SetToolWindow(ToolWindow* tool) { toolWindow = tool; }
    void setSelectedColor(COLORREF color) { this->selectedColor = color; }
    void setBuffer(BackBuffer& buffer) { backBuffer  = &buffer; }
    void setErasing(bool erase) { erasing = erase; }
    void setReplaying(bool isReplaying) { this->isReplaying = isReplaying;  }
    void SetApplication(Application* a) { app = a; }
    void ClearAll();

private:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void OnPaint(HDC hdc, const RECT& rc);
    void OnLButtonDown(int x, int y, WPARAM flags);
    void OnMouseMove(int x, int y, WPARAM flags);
    void OnLButtonUp(int x, int y, WPARAM flags);

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hwnd = nullptr;
    HINSTANCE hInstance = nullptr;
    Application* app = nullptr;

    ToolWindow* toolWindow = nullptr;
    MainWindow* mainWindow = nullptr;

    BackBuffer* backBuffer = nullptr; 
    DrawController drawCtrl;
    StrokeController strokeCtrl;
    StrokeStore store;
    COLORREF selectedColor = RGB(0, 0, 0);
    int penWidth = 2;
    bool erasing = false;
    bool isReplaying = false;
};