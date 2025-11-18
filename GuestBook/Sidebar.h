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
	void SetStroke(int s); /// 펜 굵기 설정
	int GetStroke() const { return PenStroke; } /// 펜 굵기 얻는 값
	HWND GetSliderHandle() { return hSlider; }
private:
	int SlideX, SlideTop, SlideY,SlideBottom;
	HWND hSlider = NULL;
	int PenStroke; /// 두께 설정
	INITCOMMONCONTROLSEX icc;
};

