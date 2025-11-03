#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include "Stroke.h"

class FileManager
{
public:
    FileManager() = default;

    /// 저장 및 불러오기의 진입 시점
    void StartSave(HWND dialogParent, const std::vector<Stroke>& strokes);
    void StartLoad(HWND dialogParent, std::vector<Stroke>& outStrokes, HWND drawHwnd);

private:
    /// 저장 및 불러오기 기능
    bool Save(const std::wstring& path, const std::vector<Stroke>& strokes);
    bool Load(const std::wstring& path, std::vector<Stroke>& outStrokes);

    /// 저장 및 불러오기 창 다이얼로그
    std::wstring Dialog(HWND dialogParent, bool isSave);
};
