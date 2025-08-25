#pragma once
#include <vector>
#include <WINDOWS.h>
#include <thread>
#include <mutex>
#include "StrokeStore.h"
#include "Application.h"

class ReplayController
{
	Application app;
public:
	ReplayController() = default;
	void StartReplay();
	void setApplication(Application &a) { app = a; } // app.DrawForReplay()
	StrokeStore store;
	void setStroke(int asd) {
	}
private:
	std::thread ReplayThread;
	std::mutex ReplayMutex;
	std::vector<Stroke> ReplayStroke;
};