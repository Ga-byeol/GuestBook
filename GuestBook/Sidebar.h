#pragma once
#include <Windows.h>
#include <functional>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
class Sidebar
{
public:
	Sidebar();
	void SetSlider(HWND hWnd, HINSTANCE hInstance);
	bool OnScroll(WPARAM wParam, LPARAM lParam); /// Ææ ±½±â °ª º¯°æ
	void SetStroke(int s); /// Ææ ±½±â ¼³Á¤
	int GetStroke() const { return PenStroke; } /// Ææ ±½±â ¾ò´Â °ª
	
private:
	int SlideX, SlideTop, SlideY,SlideBottom;
	HWND Slider;
	int PenStroke; /// µÎ²² ¼³Á¤
	INITCOMMONCONTROLSEX icc;
};

