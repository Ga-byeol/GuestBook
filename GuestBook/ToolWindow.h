#pragma once
#include <windows.h>
#include "ButtonController.h"

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
    void CreateButton();
   
    HWND hwnd = nullptr;
    HWND replayHwnd = nullptr;
    HINSTANCE hInstance = nullptr;
   
    DrawWindow* drawWindow = nullptr;
    
    /*ButtonController ButtonSave;
    ButtonController ButtonLoad;
    ButtonController ButtonReplay;
    ButtonController ButtonClearall;
    ButtonController ButtonErase;
    ButtonController ButtonBrush;
    ButtonController ButtonColor;*/
    ButtonController buttonController;
};

