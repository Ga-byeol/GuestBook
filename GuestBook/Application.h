#pragma once
#include <windows.h>
#include "MainWindow.h"
#include "DrawWindow.h"
#include "ToolWindow.h"

class Application {
public:
    bool Init(HINSTANCE hInstance, int nCmdShow);
    int Run();

private:
    HINSTANCE hInstance = nullptr;
    MainWindow mainWindow;
    DrawWindow drawWindow;
    ToolWindow toolWindow;
};
