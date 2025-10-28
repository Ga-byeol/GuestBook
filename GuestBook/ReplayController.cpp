#include "ReplayController.h"
#include "Application.h"

void ReplayController::StartReplay(HWND drawWindowHwnd, vector<Stroke> copyStroke, OnReplayFinishedCallback onFinished) {
	{

		if (isReplaying) return;  // 이미 실행 중이면 무시

		// 재생 상태 초기화
		isReplaying = true;
		isPaused = false;
		stopRequested = false;
	}
	// 새 스레드 생성 (이 안에서 mtx, cv를 사용해 일시정지/중단 관리)
	replayThread = std::thread([=]() {

		HDC hdc = GetDC(drawWindowHwnd);
		for (const auto& s : copyStroke) {
			HPEN pen = CreatePen(PS_SOLID, s.thickness, s.color);
			HPEN oldpen = (HPEN)SelectObject(hdc, pen);
			for (size_t i = 1; i < s.points.size(); ++i) {
				MoveToEx(hdc, s.points[i - 1].x, s.points[i - 1].y, nullptr);
				LineTo(hdc, s.points[i].x, s.points[i].y);
				Sleep(s.points[i].timestamp);
			}
			DeleteObject(pen);
			
		}
		// 원본 선에 복사 선 넣기
		// drawWindow의 isReplay = false
		onFinished(copyStroke);
		StopReplay();
		});
	replayThread.detach();
}

void ReplayController::PauseReplay() {
	rStatus = ReplayStatus::Paused;
}

void ReplayController::ResumeReplay() {
	
	rStatus = ReplayStatus::Running;
	
}

void ReplayController::StopReplay() {
	rStatus = ReplayStatus::stopped;
}