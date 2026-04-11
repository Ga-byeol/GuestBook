#include "Sidebar.h"

Sidebar::Sidebar() 
{
	SlideX = 15;
	SlideTop = 150;
	SlideY = 40;
	SlideBottom = 300;
	
	PenStroke = 1;
	hSlider = NULL;

	///공용 클래스 초기화 
	icc.dwSize = sizeof(icc);  
	icc.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc); 
}

void Sidebar::Reset() 
{
	PenStroke = INITIAL_VALUE;

	if (hSlider) {
		SendMessage(hSlider, TBM_SETPOS, TRUE, INITIAL_VALUE);

		SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 50));
		
	}

}

void Sidebar::SetSlider(HWND hWnd, HINSTANCE hInstance) 
{
	hSlider = CreateWindowExW(0, TRACKBAR_CLASS,
		L"", WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_AUTOTICKS | TBS_TOOLTIPS,
		SlideX, SlideTop, SlideY, SlideBottom,
		hWnd, nullptr, hInstance, nullptr);

	SetWindowPos(hSlider, HWND_TOP, SlideX, SlideTop, SlideY, SlideBottom, SWP_SHOWWINDOW);
	SendMessage(hSlider, TBM_SETRANGE,
		(WPARAM)TRUE,                   // (TRUE = Redraw)
		(LPARAM)MAKELONG(1, 50));       // (MAKELONG(Min, Max))
	SendMessage(hSlider, TBM_SETPOS,
		(WPARAM)TRUE,
		(LPARAM)INITIAL_VALUE);
	SendMessage(hSlider, TBM_SETPAGESIZE,
		(WPARAM)0,
		(LPARAM)1);
}

void Sidebar::SetStroke(int s) {
	if (s < 1) s = 1;	
	if (s > 30) s = 30;
	PenStroke = s;
	if (hSlider) {
		SendMessage(hSlider, TBM_SETPOS, TRUE, PenStroke);
	}
}