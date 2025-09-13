#include "ReplayController.h"
#include "Application.h"

void ReplayController::StartReplay() {
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (isReplaying) return;  // 이미 실행 중이면 무시

		replayStrokes = store->Strokes();  // 전체 복사

		store->Clear();

		// 이전 스레드 정리
		if (replayThread.joinable()) {
			stopRequested = true;
			isPaused = false;
			cv.notify_one();
			replayThread.join();
		}

		// 재생 상태 초기화
		isReplaying = true;
		isPaused = false;
		stopRequested = false;
	}
	// 새 스레드 생성 (이 안에서 mtx, cv를 사용해 일시정지/중단 관리)
	replayThread = std::thread([this]() {
		for (const auto& stroke : replayStrokes) {

			for (const auto& pt : stroke.points) {
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [this] { return !isPaused || stopRequested; });
					if (stopRequested) return;
				}
				store->ReplaySetCurrentStyle(stroke.color, stroke.thickness);
				store->ReplayCopyPointToCurrent(pt);

				app->DrawForReplay();
				Sleep(pt.timestamp);
			}
			
			store->ReplayCopyTempToStrokes();
			store->ReplayClearCurrent();
			
		}

		{
			std::lock_guard<std::mutex> lock(mtx);
			isReplaying = false;
		}
	});
		//StopReplay();
}

void ReplayController::PauseReplay() {
	std::lock_guard<std::mutex> lock(mtx);
	if (!isReplaying) return;
	isPaused = true;
}

void ReplayController::ResumeReplay() {
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (!isReplaying) return;
		isPaused = false;
	}
	cv.notify_one();
}

void ReplayController::StopReplay() {

	if (replayThread.joinable()) {
		stopRequested = true;
		isPaused = false;
		cv.notify_one();
		replayThread.join();
	}
	isReplaying = false;
	replayStrokes.clear();
	store->ReplayClearCurrent();
}