#pragma once
#include <windows.h>
#include <vector>
#include "Stroke.h"

class DrawController {
public:
    void DrawStrokes(HDC hdc,
        const std::vector<Stroke>& strokes,
        const Stroke current,
        int penWidth = 2,
        COLORREF color = RGB(30, 30, 30));
};