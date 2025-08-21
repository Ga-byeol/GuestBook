#include "DrawController.h"

namespace {
    inline void DrawPointsLine(HDC hdc, const std::vector<Point>& pts) {
        if (pts.empty()) return;
        if (pts.size() == 1) {
            MoveToEx(hdc, pts[0].x, pts[0].y, nullptr);
            LineTo(hdc, pts[0].x + 1, pts[0].y);
            return;
        }
        MoveToEx(hdc, pts[0].x, pts[0].y, nullptr);
        for (size_t i = 1; i < pts.size(); ++i) {
            LineTo(hdc, pts[i].x, pts[i].y);
        }
    }
}

void DrawController::DrawStrokes(HDC hdc,
    const std::vector<Stroke>& strokes,
    const Stroke* current,
    int penWidth,
    COLORREF color)
{

    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    for (const auto& s : strokes) {
        DrawPointsLine(hdc, s.points);
    }
    if (current) {
        DrawPointsLine(hdc, current->points);
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}