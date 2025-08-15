#pragma once
#include <vector>
#include <windows.h>

struct Point {
    int x, y;
    DWORD timestamp;
};

struct Stroke {
    std::vector<Point> points;
    COLORREF color = RGB(0, 0, 0);
    int thickness = 2;
};