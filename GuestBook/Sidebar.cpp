#include "Sidebar.h"

Sidebar::Sidebar() 
{
	SlideX = 8;
	SlideTop = 150;
	SlideY = 50;
	SlideBottom = 50 * 5;
	
	PenStroke = 1;
	

	///공용 클래스 초기화 
	icc.dwSize = sizeof(icc);  
	icc.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc); 
}

void Sidebar::SetSlider(HWND hWnd, HINSTANCE hInstance) 
{
	Slider = CreateWindowExW(0, TRACKBAR_CLASS,
		L"", WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_AUTOTICKS | TBS_TOOLTIPS,
		SlideX, SlideTop, SlideY, SlideBottom,
		hWnd, nullptr, hInstance, nullptr);

	SendMessage(Slider, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30));
	SendMessage(Slider, TBM_SETPOS, TRUE, 1);
	SetWindowPos(Slider, HWND_TOP, SlideX, SlideTop, SlideY, SlideBottom, SWP_SHOWWINDOW);
}

void Sidebar::SetStroke(int s) {
	if (s < 1) s = 1;	
	if (s > 30) s = 30;
	PenStroke = s;
	if (Slider) {
		SendMessage(Slider, TBM_SETPOS, TRUE, PenStroke);
	}
}