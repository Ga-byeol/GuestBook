#pragma once
#include <windows.h>
#include <vector>
#include "Stroke.h"


class DrawController {
public:

    void DrawStrokeOnDC(HDC hdc, const Stroke& stoke, int penWidth = 2, COLORREF color = RGB(30, 30, 30));

    void DrawAllStrokes(HDC hdc, const std::vector<Stroke>& strokes, int penWidth = 2, COLORREF color = RGB(30, 30, 30));

    void DrawAllStrokesWithCurrent(HDC hdc, const std::vector<Stroke>& strokes, const Stroke* current,
        int penWidth = 2, COLORREF color = RGB(30, 30, 30));
};
