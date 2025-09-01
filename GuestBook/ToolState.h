#pragma once
#include <windows.h>

enum class ToolType { Pen, Eraser };

class ToolState {
public:
	/// drawWindow에서 사용
	COLORREF Color() const{return color; }
	int Thickness() const{return thickness; }
	ToolType Tool() const{return tool; }


	/// ui에서 사용
	void SetTool(ToolType t) {
		if (tool == t) { return; }
		tool = t;
	}

	void SetColor(COLORREF c) {
		if (color == c) { return; }
		color = c;
	}
	void SetThickness(int t) {
		if (thickness == t) { return; }
		thickness = t;
	}

private:
	COLORREF color = RGB(0, 0, 0);
	int	thickness = 2;
	ToolType tool = ToolType::Pen;
	
};