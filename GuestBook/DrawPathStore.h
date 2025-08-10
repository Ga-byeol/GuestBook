#pragma once
#include <vector>
#include <windows.h>
#include "DrawPath.h"

class DrawPathStore {
public:
    void Begin(int x, int y);
    void Add(int x, int y);
    void End();
    void Clear();

    const std::vector<DrawPath>& Paths() const { return paths; }
    const DrawPath* Current() const { return recording ? &current : nullptr; }
    bool IsRecording() const { return recording; }

private:
    std::vector<DrawPath> paths;
    DrawPath current;
    bool recording = false;
};
