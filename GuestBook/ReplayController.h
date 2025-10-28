#pragma once
#include <vector>
#include <WINDOWS.h>
#include <thread>
#include <functional>
#include "Stroke.h";

using OnReplayFinishedCallback = std::function<void(const std::vector<Stroke>&)>;

class Application;

enum class ReplayStatus {
	Running,
	Paused,
	stopped
};

class ReplayController
{
public:
	void StartReplay(HWND drawWindowHwnd, std::vector<Stroke> copyStroke, OnReplayFinishedCallback onFinished);
	bool IsReplaying() const { return isReplaying; }

private:

	void PauseReplay();
	void ResumeReplay();
	void StopReplay();

	Application* app;
	std::thread replayThread;
	std::vector<Stroke> replayStrokes;

	ReplayStatus rStatus = ReplayStatus::stopped;
	bool isPaused = false;
	bool isReplaying = false;
	bool stopRequested = false;
};