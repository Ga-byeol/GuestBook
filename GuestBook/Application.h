#pragma once
#include <windows.h>
#include "MainWindow.h"
#include "DrawWindow.h"
#include "ToolWindow.h"
#include "ReplayController.h"
#include "ColorController.h"
#include "PenController.h"
#include "FileManager.h"
#include "resource.h"
#define SAVE 101
#define LOAD 102
#define REPLAY 103
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107
#define STOP 108
#define END 109
#define BLACK 110
#define RED 111
#define BLUE 112
#define GREEN 113
class Application {
public:
    Application() : replayController(), penBox(hInstance, nullptr) {
        auto& btnCtrl = toolWindow.GetButtonController();
        /// 저장
        btnCtrl.RegisterHandler(SAVE, [&]() {
            replayController.StopReplay();

            fileManager.StartSave(drawWindow.GetHwnd(), drawWindow.GetDrawnStrokes());
            });
        /// 불러오기
        btnCtrl.RegisterHandler(LOAD, [&]() {
            std::vector<Stroke> loadStrokes;

            replayController.StopReplay();

            drawWindow.ClearAll();

            fileManager.StartLoad(drawWindow.GetHwnd(), loadStrokes, drawWindow.GetHwnd());

            if (!loadStrokes.empty())
            {
                drawWindow.SetStrokes(loadStrokes);

                InvalidateRect(drawWindow.GetHwnd(), nullptr, TRUE);
                UpdateWindow(drawWindow.GetHwnd());
            }

            drawWindow.setReplaying(true);
            replayController.StartReplay(drawWindow.GetHwnd(), loadStrokes);
            });
        /// 재생
        btnCtrl.RegisterHandler(REPLAY, [&]() {
            if (drawWindow.GetDrawnStrokes().empty()) return 0;
            
            ReplayState state = replayController.GetState();
            HWND hReplayBtn = toolWindow.GetReplayHwnd();
            HICON hIcon = nullptr;
            if (state == ReplayState::Stopped ) {

                HWND hDrawWnd = drawWindow.GetHwnd();
                vector<Stroke> strokesCopy = drawWindow.GetDrawnStrokes();

                drawWindow.ClearScreenOnly();

                drawWindow.setReplaying(true);

                InvalidateRect(hDrawWnd, NULL, TRUE);
                UpdateWindow(hDrawWnd);

                replayController.StartReplay(hDrawWnd, strokesCopy);
                hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STOP));
                SendMessage(hReplayBtn, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
            }
            else {
                replayController.ToggleReplay();
                hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STOP));
                SendMessage(hReplayBtn, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
            }
        });
        /// 재생 중단
        btnCtrl.RegisterHandler(STOP, [&]() {
            if (!replayController.IsReplaying()) return 0;
            replayController.StopReplay();
            drawWindow.ClearAll();

            drawWindow.setReplaying(false);
            
            });
        /// 화면 초기화
        btnCtrl.RegisterHandler(CLEAR, [&]() {
            if (replayController.IsReplaying()) return 0;
            drawWindow.ClearAll();
        });
        /// 지우기
        btnCtrl.RegisterHandler(ERASE, [&]() {

            if (drawWindow.GetErasing()) {
                drawWindow.setSelectedColor(drawWindow.lastSelectedColor);
                drawWindow.SetPenStyle(penBox.LastPenNum);
            }
            else {
                drawWindow.lastSelectedColor = drawWindow.GetSelectedColor();
                penBox.LastPenNum = penBox.PenNum;
                drawWindow.setSelectedColor(RGB(255, 255, 255));
                drawWindow.SetPenStyle(PS_SOLID);
            } 
            drawWindow.setErasing(); 
        });
        /// 펜
        btnCtrl.RegisterHandler(BRUSH, [&]() {
            penBox.setDrawWindow(&drawWindow);
            penBox.PenWidth = drawWindow.GetCurrentPenWidth();
            penBox.ShowDialog();
        });
        /// 색상
        btnCtrl.RegisterHandler(COLOR, [&]() {
            if (drawWindow.GetErasing()) {
                drawWindow.setErasing();
                drawWindow.SetPenStyle(penBox.LastPenNum);
            }
                colorBox.Show();
                drawWindow.setSelectedColor(colorBox.GetColor());
        });
        btnCtrl.RegisterHandler(BLACK, [&]() {
            if (drawWindow.GetErasing()) {
                drawWindow.setErasing();
                drawWindow.SetPenStyle(penBox.LastPenNum);
            }
            drawWindow.setSelectedColor(RGB(0, 0, 0));
        });
        btnCtrl.RegisterHandler(RED, [&]() {
            if (drawWindow.GetErasing()) {
                drawWindow.setErasing();
                drawWindow.SetPenStyle(penBox.LastPenNum);
            }
            drawWindow.setSelectedColor(RGB(255, 0, 0));
            });
        btnCtrl.RegisterHandler(GREEN, [&]() {
            if (drawWindow.GetErasing()) {
                drawWindow.setErasing();
                drawWindow.SetPenStyle(penBox.LastPenNum);
            }
            drawWindow.setSelectedColor(RGB(153, 255, 51));
            });
        btnCtrl.RegisterHandler(BLUE, [&]() {
            if (drawWindow.GetErasing()) {
                drawWindow.setErasing();
                drawWindow.SetPenStyle(penBox.LastPenNum);
            }
            drawWindow.setSelectedColor(RGB(173, 216, 230));
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