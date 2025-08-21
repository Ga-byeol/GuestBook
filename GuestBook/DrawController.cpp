#include "DrawController.h"

namespace {
    void PolylineSafe(HDC hdc, const std::vector<Point>& pts) {
        if (pts.empty()) return;

        if (pts.size() == 1) {
            // 점 하나만 있을 때는 1px 라인으로 찍어줌
            MoveToEx(hdc, pts[0].x, pts[0].y, nullptr);
            LineTo(hdc, pts[0].x + 1, pts[0].y);
            return;
        }
        std::vector<POINT> gdiPts;
        gdiPts.reserve(pts.size());
        for (auto& p : pts) {
            gdiPts.push_back({ static_cast<LONG>(p.x), static_cast<LONG>(p.y) });
        }

        ::Polyline(hdc, gdiPts.data(), static_cast<int>(gdiPts.size())); 
    }
}


void DrawController::DrawStrokeOnDC(HDC hdc, const Stroke& stroke, int penWidth, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    PolylineSafe(hdc, stroke.points);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DrawController::DrawAllStrokes(HDC hdc, const std::vector<Stroke>& strokes, int penWidth, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    for (const auto& p : strokes) {
        PolylineSafe(hdc, p.points);
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DrawController::DrawAllStrokesWithCurrent(HDC hdc, const std::vector<Stroke>& stroke, const Stroke* current,
    int penWidth, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    for (const auto& p : stroke) {
        PolylineSafe(hdc, p.points);
    }
    if (current) {
        PolylineSafe(hdc, current->points);
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}
