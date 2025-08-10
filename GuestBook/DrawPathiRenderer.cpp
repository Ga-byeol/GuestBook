#include "DrawPathRenderer.h"

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


void DrawPathRenderer::DrawPathOnDC(HDC hdc, const DrawPath& path, int penWidth, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    PolylineSafe(hdc, path.points);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DrawPathRenderer::DrawAll(HDC hdc, const std::vector<DrawPath>& paths, int penWidth, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    for (const auto& p : paths) {
        PolylineSafe(hdc, p.points);
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DrawPathRenderer::DrawAllWithCurrent(HDC hdc, const std::vector<DrawPath>& paths, const DrawPath* current,
    int penWidth, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, penWidth, color);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    for (const auto& p : paths) {
        PolylineSafe(hdc, p.points);
    }
    if (current) {
        PolylineSafe(hdc, current->points);
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}
