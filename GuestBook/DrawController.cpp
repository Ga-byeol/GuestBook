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
    const Stroke current,
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
    if (!current.points.empty()) { ///현재 마우스를 눌러서 그리는 점의 정보
        LOGBRUSH style = {};
        style.lbStyle = BS_SOLID;
        style.lbColor = current.color;

        HPEN pen = ExtCreatePen(PS_GEOMETRIC | current.penStyle, current.penWidth, &style, 0, nullptr);
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        DrawPointsLine(hdc, current.points);
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