#pragma once
#include <vector>
#include <mutex>
#include <windows.h>

#include "Stroke.h"

class StrokeStore {
public:
    void Begin(int x, int y, COLORREF color);
    void Add(int x, int y);
    void End();
    void Clear();
    void ReplayCopyStroke(Stroke& dest, const Stroke& src);
    void ReplaySetCurrentStyle(COLORREF color, int thickness);
    void ReplayCopyPointToCurrent(Point pt);
    void ReplayCopyTempToStrokes();
    void ReplayClearCurrent();


    const std::vector<Stroke> Strokes() const { return strokes; }
    const Stroke Current() const { return current; }
    bool IsRecording() const { return recording; }

private:

    std::mutex mtx;
    std::vector<Stroke> strokes;
    
    Stroke current;
    DWORD lastTime;
    bool recording = false;
};
