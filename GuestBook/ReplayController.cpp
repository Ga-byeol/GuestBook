#include "ReplayController.h"
#include "Application.h"

void ReplayController::StartReplay() {
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (isReplaying) return;  // �̹� ���� ���̸� ����

		replayStrokes = store->Strokes();  // ��ü ����

		store->Clear();

		// ���� ������ ����
		if (replayThread.joinable()) {
			stopRequested = true;
			isPaused = false;
			cv.notify_one();
			replayThread.join();
		}

		// ��� ���� �ʱ�ȭ
		isReplaying = true;
		isPaused = false;
		stopRequested = false;
	}
	// �� ������ ���� (�� �ȿ��� mtx, cv�� ����� �Ͻ�����/�ߴ� ����)
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