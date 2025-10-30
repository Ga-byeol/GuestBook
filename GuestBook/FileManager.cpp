#include "FileManager.h"

std::wstring FileManager::Dialog(HWND dialogParent, bool isSave)
{
    OPENFILENAME ofn;
    wchar_t filePath[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn)); /// ofn 구조체 안의 메모리 블록을 0으로 초기화
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = dialogParent;
    ofn.lpstrFilter = L"Stroke Files (*.dat)\0*.dat\0All Files\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrDefExt = L"dat";
    ofn.lpstrInitialDir = L"..\\file\\";

    if (isSave)
    {
        ofn.lpstrTitle = L"그림 저장하기";
        if (GetSaveFileName(&ofn)) { return filePath; }
    }
    else
    {
        ofn.lpstrTitle = L"그림 불러오기";
        if (GetOpenFileName(&ofn)) { return filePath; }
    }

    return L""; /// 취소 시 빈 문자열 반환
}

/// 주의점 : 저장 및 불러오기 시 쓰는 순서와 읽는 순서 동일해야 함
///          Stroke 내부 순서 변경 시 Save, Load도 함께 수정 필요

bool FileManager::Save(const std::wstring& path, const std::vector<Stroke>& strokes)
{
    std::wofstream saveFile(path);
    if (!saveFile.is_open()) return false;

    saveFile << strokes.size() << L"\n";  /// Stroke의 개수가 먼저 저장

    for (const auto& s : strokes)
    {
        saveFile << s.color << L' ' << s.thickness << L' ' << s.points.size() << L'\n';

        for (const auto& p : s.points)
        {
            /// 시간 값 까지 모두 저장
            saveFile << p.x << L' ' << p.y << L' ' << p.timestamp << L'\n';
        }
    }

    saveFile.close();
    return true;
}

bool FileManager::Load(const std::wstring& path, StrokeController* strokeCtrl)
{
    std::wifstream loadFile(path);
    if (!loadFile.is_open()) return false;

    std::vector<Stroke> loadStroke; /// 저장용 Stroke 벡터
    size_t strokeCount; /// 선의 개수
    loadFile >> strokeCount;

    for (size_t i = 0; i < strokeCount; ++i)
    {
        Stroke s;
        size_t pointCount; /// 점의 개수
        loadFile >> s.color >> s.thickness >> pointCount;

        for (size_t j = 0; j < pointCount; ++j)
        {
            Point p;
            loadFile >> p.x >> p.y >> p.timestamp;
            s.points.push_back(p);
        }

        loadStroke.push_back(s);
    }

    strokeCtrl->setStrokes(loadStroke);
    loadFile.close();
    return true;
}

void FileManager::StartSave(HWND dialogParent, const std::vector<Stroke>& strokes)
{
    std::wstring path = Dialog(dialogParent, true); /// GetSaveFileName
    if (path.empty()) { return; }

    if (Save(path, strokes))
    {
        MessageBox(dialogParent, L"저장 완료!", L"파일 저장", MB_OK);
    }
    else
    {
        MessageBox(dialogParent, L"저장 실패!", L"파일 저장", MB_OK);
    }
}

void FileManager::StartLoad(HWND dialogParent, StrokeController* strokeCtrl, HWND drawHwnd)
{
    std::wstring path = Dialog(dialogParent, false); /// GetOpenFileName
    if (path.empty()) { return; }

    if (Load(path, strokeCtrl))
    {
        MessageBox(dialogParent, L"불러오기 완료!", L"파일 불러오기", MB_OK);

        /// DrawWindow의 화면 새로고침
        InvalidateRect(drawHwnd, nullptr, TRUE);
        UpdateWindow(drawHwnd);
    }
    else
    {
        MessageBox(dialogParent, L"불러오기 실패!", L"파일 불러오기", MB_OK);
    }
}