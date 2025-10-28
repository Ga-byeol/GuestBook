#pragma once
#include <vector>
#include <WINDOWS.h>
#include <thread>
#include "Stroke.h"

using OnReplayFinishedCallback = std::function<void(const vector<Stroke>&)>;

class Application;

enum class ReplayStatus {
	Running,
	Paused,
	stopped
};

class ReplayController
{
public:
	ReplayController() { rStatus = ReplayStatus::stopped; }
	ReplayController(Application* a, StrokeStore* s) : app(a), store(s) {};
	~ReplayController() { StopReplay(); }
	void StartReplay(HWND drawWindowHwnd, vector<Stroke> copyStroke, OnReplayFinishedCallback onFinished);
	

private:

	void PauseReplay();
	void ResumeReplay();
	void StopReplay();

	Application* app;
	StrokeStore* store = nullptr;
	std::thread replayThread;
	std::vector<Stroke> replayStrokes;

	ReplayStatus rStatus = ReplayStatus::stopped;
	bool isPaused = false;
	bool isReplaying = false;
	bool stopRequested = false;
};