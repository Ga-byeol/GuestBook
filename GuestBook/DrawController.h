#pragma once
#include <windows.h>
#include <vector>
#include "StructStroke.h"


class DrawController {
public:

    void DrawStrokeOnDC(HDC hdc, const StructStroke& stoke, int penWidth = 2, COLORREF color = RGB(30, 30, 30));

    void DrawAllStrokes(HDC hdc, const std::vector<StructStroke>& strokes, int penWidth = 2, COLORREF color = RGB(30, 30, 30));

    void DrawAllStrokesWithCurrent(HDC hdc, const std::vector<StructStroke>& strokes, const StructStroke* current,
        int penWidth = 2, COLORREF color = RGB(30, 30, 30));
};
