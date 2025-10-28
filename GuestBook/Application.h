#pragma once
#include <windows.h>
#include "MainWindow.h"
#include "DrawWindow.h"
#include "ToolWindow.h"
#include "ReplayController.h"
#include "ColorController.h"
#define SAVE 101
#define LOAD 102
#define REPLAY 103
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107

class Application {
public:
    Application() : replayController(){
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
            OutputDebugString(L"replay clicked\n");

            drawWindow.setReplaying(true);

            HWND hDrawWnd = drawWindow.GetHwnd();
            vector<Stroke> strokesCopy = drawWindow.GetDrawnStrokes();

            drawWindow.ClearAll();

            InvalidateRect(hDrawWnd, NULL, TRUE);
            UpdateWindow(hDrawWnd);

            auto onFinishCallback = [this, hDrawWnd](const vector<Stroke>& replayedData) {

                // 이 코드는 나중에 Replay 스레드가 호출해 줄 것임
                
                // drawWindow.SetStrokes(replayedData);// 1. 원본 데이터 복원
                
                drawWindow.setReplaying(false);      // 2. 리플레이 모드 해제

                InvalidateRect(hDrawWnd, NULL, FALSE); // 3. 화면 갱신
                };

                replayController.StartReplay(hDrawWnd, strokesCopy, onFinishCallback);
            });
        btnCtrl.RegisterHandler(CLEAR, [&]() {
        ///    MessageBox(nullptr, L"전체 지우기 버튼", L"TOOL창", MB_OK);
            drawWindow.ClearAll();
            });
        btnCtrl.RegisterHandler(ERASE, [&]() {
        ///    MessageBox(nullptr, L"지우기 버튼", L"TOOL창", MB_OK);
            drawWindow.setSelectedColor(RGB(255, 255, 255));
            });
        btnCtrl.RegisterHandler(BRUSH, [&]() {
            MessageBox(nullptr, L"브러쉬 버튼", L"TOOL창", MB_OK);
            });
        btnCtrl.RegisterHandler(COLOR, [&]() {
                colorBox.Show();
                drawWindow.setSelectedColor(colorBox.GetColor());
            });
    }
    bool Init(HINSTANCE hInstance, int nCmdShow);
    int Run();
    
    bool IsReplaying() const { return replayController.IsReplaying(); };

private:
    HINSTANCE hInstance = nullptr;
    MainWindow mainWindow;
    DrawWindow drawWindow;
    ToolWindow toolWindow;
    ReplayController replayController; 
    ColorController colorBox;

};