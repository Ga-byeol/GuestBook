#pragma once
#include <windows.h>
#include "MainWindow.h"
#include "DrawWindow.h"
#include "ToolWindow.h"
#include "ReplayController.h"
#include "ColorController.h"
#include "PenController.h"
#include "FileManager.h"
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
            fileManager.StartSave(drawWindow.GetHwnd(), drawWindow.GetDrawnStrokes());
            });
        btnCtrl.RegisterHandler(LOAD, [&]() {
            std::vector<Stroke> loadStrokes;

            /// 불러오기 때에는 리플레이 중지
            replayController.StopReplay();

            /// 기존에 그려져 있던 그림을 모두 제거
            drawWindow.ClearAll();

            /// 파일에서 읽기
            fileManager.StartLoad(drawWindow.GetHwnd(), loadStrokes, drawWindow.GetHwnd());

            /// 파일이 정상적으로 불러와졌다면 DrawWindow에 전달
            if (!loadStrokes.empty())
            {
                drawWindow.SetStrokes(loadStrokes);

                InvalidateRect(drawWindow.GetHwnd(), nullptr, TRUE);
                UpdateWindow(drawWindow.GetHwnd());
            }

            /// 불러와진 그림을 즉시 리플레이
            /// 리플레이 중일 때에는 플래그 막아 펜 못 그리게 하기
            drawWindow.setReplaying(true);
            replayController.StartReplay(drawWindow.GetHwnd(), loadStrokes);
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
    FileManager fileManager;
};