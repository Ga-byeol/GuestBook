#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include "Stroke.h"
#include "StrokeController.h"

class FileManager
{
public:
    FileManager() = default;

    /// 저장 및 불러오기의 진입 시점
    void StartSave(HWND dialogParent, const std::vector<Stroke>& strokes);
    void StartLoad(HWND dialogParent, StrokeController* strokeCtrl, HWND drawHwnd);

private:
    /// 저장 및 불러오기 기능
    bool Save(const std::wstring& path, const std::vector<Stroke>& strokes);
    bool Load(const std::wstring& path, StrokeController* strokeCtrl);

    /// 저장 및 불러오기 창 다이얼로그
    std::wstring Dialog(HWND dialogParent, bool isSave);
};
