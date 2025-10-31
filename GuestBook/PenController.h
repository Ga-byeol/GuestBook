#pragma once
#include <Windows.h>

class DrawWindow;

class PenController
{
public:
	PenController(HINSTANCE hInst, HWND parent);
	void ShowDialog(); /// 다이어로그 띄우기
	void setDrawWindow(DrawWindow* dw) { drawWindow = dw; };
	int PenNum = 0;
	int PenWidth = 1;

private:
	HINSTANCE hInstance = nullptr;
	HWND parentHwnd = nullptr;
	DrawWindow* drawWindow = nullptr;
	static INT_PTR CALLBACK DialogMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

