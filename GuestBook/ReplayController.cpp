#include "ReplayController.h"
#include "Application.h"

void ReplayController::StartReplay(HWND hDrawWnd, vector<Stroke> copyStroke, OnReplayFinishedCallback onFinished) {
	{

		if (isReplaying) return;  // 이미 실행 중이면 무시

		// 재생 상태 초기화
		isReplaying = true;
		isPaused = false;
		stopRequested = false;
	}
	// 새 스레드 생성 (이 안에서 mtx, cv를 사용해 일시정지/중단 관리)
	replayThread = std::thread([=]() {

		for (const auto& s : copyStroke) {
			for (size_t i = 1; i < s.points.size(); ++i) {
				Sleep(s.points[i].timestamp);

				HDC hdc = GetDC(hDrawWnd);
				HPEN pen = CreatePen(PS_SOLID, s.thickness, s.color);
				HPEN oldpen = (HPEN)SelectObject(hdc, pen);
				
				MoveToEx(hdc, s.points[i - 1].x, s.points[i - 1].y, nullptr);
				LineTo(hdc, s.points[i].x, s.points[i].y);
				
				SelectObject(hdc, oldpen);

				DeleteObject(pen);
				ReleaseDC(hDrawWnd, hdc);
			}
		}
		// 원본 선에 복사 선 넣기
		// drawWindow의 isReplay = false

		/*if (onFinished) {
			onFinished(copyStroke);
		}*/

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