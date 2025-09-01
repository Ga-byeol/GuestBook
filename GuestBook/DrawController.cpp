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
    // 브러시는 비우고(채우기 없음) 유지
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    // 1) 저장된 선들 = 각 Stroke 고유 스타일로 그림
    for (const auto& s : strokes) {
        if (s.points.empty()) continue;
        HPEN pen = CreatePen(PS_SOLID, s.thickness, s.color);
        HGDIOBJ oldPen = SelectObject(hdc, pen);

        // 기존 DrawPointsLine 재사용
        DrawPointsLine(hdc, s.points);

        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }

    // 2) 현재 선 = 현재 툴의 스타일로 그림 (penWidth/color 인자 사용)
    if (current && !current->points.empty()) {
        HPEN pen = CreatePen(PS_SOLID, penWidth, color);
        HGDIOBJ oldPen = SelectObject(hdc, pen);

        DrawPointsLine(hdc, current->points);

        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }

    SelectObject(hdc, oldBrush);
}