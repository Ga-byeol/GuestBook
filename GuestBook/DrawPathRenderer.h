#pragma once
#include <windows.h>
#include <vector>
#include "DrawPath.h"


class DrawPathRenderer {
public:

    void DrawPathOnDC(HDC hdc, const DrawPath& path, int penWidth = 2, COLORREF color = RGB(30, 30, 30));

    void DrawAll(HDC hdc, const std::vector<DrawPath>& paths, int penWidth = 2, COLORREF color = RGB(30, 30, 30));

    void DrawAllWithCurrent(HDC hdc, const std::vector<DrawPath>& paths, const DrawPath* current,
        int penWidth = 2, COLORREF color = RGB(30, 30, 30));
};