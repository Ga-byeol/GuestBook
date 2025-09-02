#pragma once
#include <windows.h>
#include "MainWindow.h"
#include "DrawWindow.h"
#include "ToolWindow.h"
#include "ReplayController.h"

#define SAVE 101
#define LOAD 102
#define REPLAY 103
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107

class Application {
public:
    Application() {
        // ToolWindow 안의 ButtonController에 접근
        auto& btnCtrl = toolWindow.GetButtonController();

        // 버튼 ID에 맞는 동작 등록
        btnCtrl.RegisterHandler(SAVE, [&]() {
            MessageBox(nullptr, L"저장 버튼", L"TOOL창", MB_OK);
            });
        btnCtrl.RegisterHandler(LOAD, [&]() {
            MessageBox(nullptr, L"불러오기 버튼", L"TOOL창", MB_OK);
            });
        btnCtrl.RegisterHandler(REPLAY, [&]() {
            replayController.StartReplay();
            });
        btnCtrl.RegisterHandler(CLEAR, [&]() {
            MessageBox(nullptr, L"전체 지우기 버튼", L"TOOL창", MB_OK);
            });
        btnCtrl.RegisterHandler(ERASE, [&]() {
            MessageBox(nullptr, L"지우기 버튼", L"TOOL창", MB_OK);
            });
        btnCtrl.RegisterHandler(BRUSH, [&]() {
            MessageBox(nullptr, L"브러쉬 버튼", L"TOOL창", MB_OK);
            });
        btnCtrl.RegisterHandler(COLOR, [&]() {
            MessageBox(nullptr, L"색상 버튼", L"TOOL창", MB_OK);
            });

    }
    bool Init(HINSTANCE hInstance, int nCmdShow);
    int Run();
    void DrawForReplay();
private:
    HINSTANCE hInstance = nullptr;
    MainWindow mainWindow;
    DrawWindow drawWindow;
    ToolWindow toolWindow;
    ReplayController replayController;
};
