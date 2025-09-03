#include "ReplayController.h"
#include "Application.h"

/*void ReplayController::StartReplay() {
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (isReplaying) return;
	
		if (store->Strokes().empty()) {
			return;
		}
	}

	replayStroke = store->Strokes();
	store->Clear();
	app->DrawForReplay();

	if (replayThread.joinable()) {
		stopRequested = true;
		isPaused = false;
		cv.notify_one();
		replayThread.join();
	}

	isReplaying = true;
	isPaused = false;
	stopRequested = false;

	OutputDebugString(L"1 called\n");

	replayThread = std::thread([this]() {
			OutputDebugString(L"2 called\n");
		for (const auto& stroke : replayStroke) {
			OutputDebugString(L"3 called\n");

			Stroke tempStroke;

			tempStroke.color = stroke.color;
			tempStroke.thickness = stroke.thickness;

			for (const auto& pt : stroke.points) {
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [this] { return !isPaused || stopRequested; });
					if (stopRequested) return;
				}
				tempStroke.points.push_back(pt);

				store->ReplayCopyPointToCurrent(tempStroke);

				app->DrawForReplay();

				Sleep(pt.timestamp);
			}
			store->ReplayCopyTempToStrokes(tempStroke);
			store->ReplayClearCurrent();
		};
		// replay end
		StopReplay();

	});
}*/
void ReplayController::StartReplay() {
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (isReplaying) return;  // �̹� ���� ���̸� ����

		if (replayStrokes.empty()) {
			replayStrokes = store->Strokes();  // ��ü ����
		}

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
			Stroke tempStroke;
			tempStroke.color = stroke.color;
			tempStroke.thickness = stroke.thickness;

			for (const auto& pt : stroke.points) {
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [this] { return !isPaused || stopRequested; });
					if (stopRequested) return;
				}
				tempStroke.points.push_back(pt);
				{
					//std::lock_guard<std::mutex> lock(mtx);
					store->ReplayCopyPointToCurrent(tempStroke);
				}

				app->DrawForReplay();
				Sleep(pt.timestamp);
			}

			{
				//std::lock_guard<std::mutex> lock(mtx);
				store->ReplayCopyTempToStrokes(tempStroke);
				store->ReplayClearCurrent();
			}
		}

		{
			std::lock_guard<std::mutex> lock(mtx);
			isReplaying = false;
		}
		app->DrawForReplay();
	});
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
	//std::lock_guard<std::mutex> lock(mtx);

	if (replayThread.joinable()) {
		stopRequested = true;
		isPaused = false;
		cv.notify_one();
		replayThread.join();
	}
	isReplaying = false;
	replayStrokes.clear();
	store->ReplayClearCurrent();
	OutputDebugString(L"1 called\n");

}