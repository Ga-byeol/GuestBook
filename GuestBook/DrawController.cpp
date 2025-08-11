#include "DrawController.h"

namespace {
    void PolylineSafe(HDC hdc, const std::vector<POINT>& pts) {
        if (pts.empty()) return;

        if (pts.size() == 1) {
            // 점 하나만 있을 때는 1px 라인으로 찍어줌
            MoveToEx(hdc, pts[0].x, pts[0].y, nullptr);
            LineTo(hdc, pts[0].x + 1, pts[0].y);
            return;
        }
        ::Polyline(hdc, pts.data(), static_cast<int>(pts.size()));
    }
}


void DrawController::DrawStrokeOnDC(HDC hdc, const StructStroke& stroke, int penWidth, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    PolylineSafe(hdc, stroke.points);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DrawController::DrawAllStrokes(HDC hdc, const std::vector<StructStroke>& strokes, int penWidth, COLORREF color) {
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

void DrawController::DrawAllStrokesWithCurrent(HDC hdc, const std::vector<StructStroke>& stroke, const StructStroke* current,
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
