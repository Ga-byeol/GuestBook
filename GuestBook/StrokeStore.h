#pragma once
#include <vector>
#include <windows.h>
#include "Stroke.h"

class StrokeStore {
public:
    void Begin(int x, int y);
    void Add(int x, int y);
    void End();
    void Clear();
    void ReplayCopyStroke(Stroke& dest, const Stroke& src);
    void ReplaySetCurrentStyle(COLORREF color, int thickness);
    void ReplayCopyPointToCurrent(int x, int y);
    void ReplayCopyTempToStrokes(Stroke& s);
    void ReplayClearCurrent();


    const std::vector<Stroke>& Strokes() const { return strokes; }
    const Stroke* Current() const { return recording ? &current : nullptr; }
    bool IsRecording() const { return recording; }

private:
    std::vector<Stroke> strokes;
    Stroke current;
    bool recording = false;
};
