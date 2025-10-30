#pragma once
#include <windows.h>
#include "MainWindow.h"
#include "DrawWindow.h"
#include "ToolWindow.h"
#include "ReplayController.h"
#include "ColorController.h"
#include "PenController.h"
#define SAVE 101
#define LOAD 102
#define REPLAY 103
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107

class Application {
public:
    Application() : replayController(), penBox(hInstance, nullptr) {
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
            
            ReplayState state = replayController.GetState();

            if (state == ReplayState::Stopped) {
                // [새로 재생]
                // (이전 코드: 데이터 복사, ClearAll, setReplaying(true), 화면 클리어...)
                // (onFinish 콜백은 *필요 없음*. 어차피 무한 루프이므로)

                HWND hDrawWnd = drawWindow.GetHwnd();
                vector<Stroke> strokesCopy = drawWindow.GetDrawnStrokes();

                drawWindow.ClearAll();

                drawWindow.setReplaying(true);

                InvalidateRect(hDrawWnd, NULL, TRUE);
                UpdateWindow(hDrawWnd);

                // ★★★ 콜백(onFinished) 없이 스레드 시작 ★★★
                replayController.StartReplay(hDrawWnd, strokesCopy);

            }
            else {
                // [일시정지/재개 토글]
                replayController.ToggleReplay();
            }
        });
        btnCtrl.RegisterHandler(CLEAR, [&]() {
        ///    MessageBox(nullptr, L"전체 지우기 버튼", L"TOOL창", MB_OK);
            drawWindow.setReplaying(false);
            replayController.StopReplay();
            drawWindow.ClearAll();
        });
        btnCtrl.RegisterHandler(ERASE, [&]() {
        ///    MessageBox(nullptr, L"지우기 버튼", L"TOOL창", MB_OK);
            drawWindow.setSelectedColor(RGB(255, 255, 255));
        });
        btnCtrl.RegisterHandler(BRUSH, [&]() {
            penBox.setDrawWindow(&drawWindow);
            penBox.ShowDialog();
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
    PenController penBox;
};