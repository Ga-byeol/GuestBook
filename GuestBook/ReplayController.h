#pragma once
#include <vector>
#include <WINDOWS.h>
#include <thread>
#include <mutex>
#include "StrokeStore.h"
#include "Application.h"

class ReplayController
{
public:
	ReplayController() = default;
	void StartReplay();
	void SetApplication(Application* a) { app = a; } // app.DrawForReplay()
	bool ReplayRecording = FALSE;
	void ClickReplay();
	void ReplayPause();
	StrokeStore store;
private:
	Application* app;
	std::thread ReplayThread;
	std::mutex ReplayMutex;
	std::vector<Stroke> ReplayStroke;
};