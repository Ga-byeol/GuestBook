#pragma once
#include <windows.h>
#include "DrawWindow.h"
#include "ToolWindow.h"
#include "BackBuffer.h"
#include "UIDrawBridge.h" 
#include "StrokeStore.h" 

class MainWindow
{
public:
	MainWindow() = default;
	bool Create(HINSTANCE hInstance, int nCmdShow);
	HWND GetHwnd() const { return hwnd; }

	void SetDrawWindow(DrawWindow* draw) {
		drawWindow = draw; 
		ConnectUIDraw();
	}
	void SetToolWindow(ToolWindow* tool) {
		toolWindow = tool; 
		ConnectUIDraw();
	}

	void Show(int nCmdShow = SW_SHOW);
	void ResizeChildren();

	/// BackBuffer 필요시 호출 getter
	BackBuffer* GetBackBuffer() { return &backbuffer; }
	BackBuffer* GetFrontBuffer() { return &frontbuffer; }

	void ConnectUIDraw();

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	HWND hwnd = nullptr;
	HINSTANCE hInstance = nullptr;

	DrawWindow* drawWindow = nullptr;
	ToolWindow* toolWindow = nullptr;
	BackBuffer backbuffer;
	BackBuffer frontbuffer;
	UIDrawBridge bridge{};
};

