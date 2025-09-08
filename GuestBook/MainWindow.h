#pragma once
#include <windows.h>
#include "DrawWindow.h"
#include "ToolWindow.h"
#include "BackBuffer.h"

class MainWindow
{
public:
	MainWindow() = default;
	bool Create(HINSTANCE hInstance, int nCmdShow);
	HWND GetHwnd() const { return hwnd; }

	void SetDrawWindow(DrawWindow* draw) { drawWindow = draw; }
	void SetToolWindow(ToolWindow* tool) { toolWindow = tool; }

	void Show(int nCmdShow = SW_SHOW);
	void ResizeChildren();

	BackBuffer& GetBackBuffer() { return back; }

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	HWND hwnd = nullptr;
	HINSTANCE hInstance = nullptr;

	DrawWindow* drawWindow = nullptr;
	ToolWindow* toolWindow = nullptr;
	BackBuffer back;
};

