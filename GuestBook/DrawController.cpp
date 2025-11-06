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
    int penWidth,
    COLORREF color
    )
{

    for (const auto& s : strokes) { /// 다시 그릴 때 사용하는 역할
        LOGBRUSH style = {};
        style.lbStyle = BS_SOLID;
        style.lbColor = s.color;
        /// ExtCreatePen 사용
        HPEN pen = ExtCreatePen(PS_GEOMETRIC | s.penStyle, s.penWidth, &style, 0, nullptr);
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        DrawPointsLine(hdc, s.points);
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }
}

void DrawController::DrawLatestStroke(HDC hdc, const Stroke& stroke, int penWidth, COLORREF color) { /// 점과 점을 이어주는 역할
    if (stroke.points.size() < 2) return; 

    LOGBRUSH style = {};
    style.lbStyle = BS_SOLID;
    style.lbColor = stroke.color;
    HPEN pen = ExtCreatePen(PS_GEOMETRIC | stroke.penStyle, stroke.penWidth,&style,0,nullptr);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    
    const Point& prevPoint = stroke.points[stroke.points.size() - 2];
    const Point& lastPoint = stroke.points.back();

    MoveToEx(hdc, prevPoint.x, prevPoint.y, nullptr);
    LineTo(hdc, lastPoint.x, lastPoint.y);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

// 점(dot)을 그리는 새 함수
void DrawController::DrawCursorDot(HDC hdc, POINT pos, int penWidth, COLORREF color, bool isErasing) {

    // 지우개 모드일 때는 하얀 사각형으로 (예시)
    if (isErasing) {
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
        RECT rc = { pos.x - penWidth / 2, pos.y - penWidth / 2, pos.x + penWidth / 2, pos.y + penWidth / 2 };
        Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
        return;
    }

    // 펜 모드일 때는 색상과 굵기에 맞는 '원'
    // (ExtCreatePen은 굵은 펜의 테두리만 그림. 속을 채워야 함)

    HBRUSH hBrush = CreateSolidBrush(color); // 1. 펜 색상과 '같은' 브러시
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    HPEN hPen = (HPEN)GetStockObject(NULL_PEN); // 2. 테두리(Pen)는 없음
    HGDIOBJ oldPen = SelectObject(hdc, hPen);

    int radius = penWidth / 2;
    if (radius < 2) radius = 2; // (최소 크기 보장)

    // 3. 원 그리기 (Ellipse)
    Ellipse(hdc, pos.x - radius, pos.y - radius, pos.x + radius, pos.y + radius);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}