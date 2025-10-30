#pragma once
#include <vector>
#include <WINDOWS.h>
#include <thread>
#include <functional>
#include <atomic>
#include "Stroke.h";

using OnReplayFinishedCallback = std::function<void(const std::vector<Stroke>&)>;

class Application;

enum class ReplayState {
	Stopped,  // 완전 멈춤 (초기 상태)
	Running,  // 재생 중
	Paused,   // 일시정지
	Stopping  // 중단 요청 (Clear 버튼)
};

class ReplayController
{
public:
	void StartReplay(HWND drawWindowHwnd, std::vector<Stroke> copyStroke);
	bool IsReplaying() const { return isReplaying; }
	ReplayState GetState() const { return r_state; }
	void ToggleReplay();
	void StopReplay();

private:


	Application* app;
	std::thread replayThread;
	std::vector<Stroke> replayStrokes;

	std::atomic<ReplayState> r_state = ReplayState::Stopped;
	bool isPaused = false;
	bool isReplaying = false;
	bool stopRequested = false;
};