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
	ReplayController(Application* a, StrokeStore* s) : app(a), store(s) {};
	~ReplayController() { StopReplay(); }
	void StartReplay();

private:
	void PauseReplay();
	void ResumeReplay();
	void StopReplay();

	Application* app;
	StrokeStore* store = nullptr;
	std::thread replayThread;
	std::mutex mtx;
	std::condition_variable cv;
	std::vector<Stroke> replayStrokes;
	
	bool isPaused = false;
	bool isReplaying = false;
	bool stopRequested = false;
};