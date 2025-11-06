#pragma once
#include <windows.h>
#include <vector>
#include <condition_variable>
#include "Stroke.h"
#include "DrawController.h"
#include "StrokeController.h"
#include "BackBuffer.h"
#include "Sidebar.h"
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
    BOOL GetErasing() const { return erasing;  }
    COLORREF GetSelectedColor() const { return selectedColor; }
    const std::vector<Stroke>& GetDrawnStrokes() const { return strokeCtrl.Strokes(); }
    void SetStrokes(std::vector<Stroke> strokes) { strokeCtrl.setStrokes(strokes); }
    void SetPenStyle(int PenNum); /// 브러쉬 컨트롤러 다이얼로그에서 받은 넘버
    void SetPenWidth(int PenWidth); /// 브러쉬 컨트롤러 다이얼로그에서 받은 두께


    void SetToolWindow(ToolWindow* tool) { toolWindow = tool; }
    void setSelectedColor(COLORREF color) { this->selectedColor = color; }
    void setBuffer(BackBuffer& buffer) { backBuffer  = &buffer; }
    void setErasing() { erasing = !erasing; }
    void setReplaying(bool isReplaying) { this->isReplaying = isReplaying;  }
    void SetApplication(Application* a) { app = a; }
    void ClearAll();

    COLORREF lastSelectedColor = RGB(0, 0, 0);

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
    COLORREF selectedColor = RGB(0, 0, 0);
    int penWidth = 2;
    bool erasing = false;
    bool isReplaying = false;
    int currentPenStyle = PS_SOLID;
    int currentPenWidth = 5;
};