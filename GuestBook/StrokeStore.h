#pragma once
#include <vector>
#include <windows.h>
#include "StructStroke.h"

class StrokeStore {
public:
    void Begin(int x, int y);
    void Add(int x, int y);
    void End();
    void Clear();

    const std::vector<StructStroke>& Strokes() const { return strokes; }
    const StructStroke* Current() const { return recording ? &current : nullptr; }
    bool IsRecording() const { return recording; }

private:
    std::vector<StructStroke> strokes;
    StructStroke current;
    bool recording = false;
};
