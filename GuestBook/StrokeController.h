#pragma once
#include <vector>
#include <windows.h>
#include "Stroke.h"

class StrokeController {
public:
    void Begin(int x, int y);
    void Add(int x, int y);
    void End();
    void Clear();
    void AddPoint(Point p);
    void AddStroke();

    const std::vector<Stroke>& Strokes() const { return strokes; }
    const Stroke* Current() const { return recording ? &current : nullptr; }
    bool IsRecording() const { return recording; }

private:
    std::vector<Stroke> strokes;
    Stroke current;
    bool recording = false;
};
