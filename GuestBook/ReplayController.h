#pragma once
#include <vector>
#include <WINDOWS.h>
#include <thread>
#include <mutex>
#include "StrokeStore.h"

class Application;

class ReplayController
{
public:
	ReplayController() = default;
	ReplayController(Application* a) : app(a) {};
	void StartReplay();
	bool ReplayRecording = FALSE;
	void ClickReplay();
	std::condition_variable Pause;
	
	StrokeStore store;
private:
	Application* app;
	std::thread ReplayThread;
	std::mutex ReplayMutex;
	std::vector<Stroke> ReplayStroke;
};