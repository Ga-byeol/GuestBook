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
        // ToolWindow ���� ButtonController�� ����
        auto& btnCtrl = toolWindow.GetButtonController();

        // ��ư ID�� �´� ���� ���
        btnCtrl.RegisterHandler(SAVE, [&]() {
            MessageBox(nullptr, L"���� ��ư", L"TOOLâ", MB_OK);
            });
        btnCtrl.RegisterHandler(LOAD, [&]() {
            MessageBox(nullptr, L"�ҷ����� ��ư", L"TOOLâ", MB_OK);
            });
        btnCtrl.RegisterHandler(REPLAY, [&]() {
            replayController.StartReplay();
            });
        btnCtrl.RegisterHandler(CLEAR, [&]() {
            MessageBox(nullptr, L"��ü ����� ��ư", L"TOOLâ", MB_OK);
            });
        btnCtrl.RegisterHandler(ERASE, [&]() {
            MessageBox(nullptr, L"����� ��ư", L"TOOLâ", MB_OK);
            });
        btnCtrl.RegisterHandler(BRUSH, [&]() {
            MessageBox(nullptr, L"�귯�� ��ư", L"TOOLâ", MB_OK);
            });
        btnCtrl.RegisterHandler(COLOR, [&]() {
            MessageBox(nullptr, L"���� ��ư", L"TOOLâ", MB_OK);
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
