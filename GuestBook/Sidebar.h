#pragma once
#include <Windows.h>
#include <functional>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#define INITIAL_VALUE 5
class Sidebar
{
public:
	Sidebar();
	void SetSlider(HWND hWnd, HINSTANCE hInstance);
	bool OnScroll(WPARAM wParam, LPARAM lParam); /// Ææ ±½±â °ª º¯°æ
	void SetStroke(int s); /// Ææ ±½±â ¼³Á¤
	int GetStroke() const { return PenStroke; } /// Ææ ±½±â ¾ò´Â °ª
	HWND GetSliderHandle() { return hSlider; }
private:
	int SlideX, SlideTop, SlideY,SlideBottom;
	HWND hSlider;
	int PenStroke; /// µÎ²² ¼³Á¤
	INITCOMMONCONTROLSEX icc;
};

