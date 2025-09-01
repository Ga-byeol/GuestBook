#include "ReplayController.h"

void ReplayController::ClickReplay() {
	if (ReplayRecording == TRUE) ReplayRecording = FALSE;
	else ReplayRecording = TRUE;
}

void ReplayController::StartReplay() {
	if (store.Strokes().empty()) {
		return;
	}
	ReplayThread = std::thread([this]() {
		std::unique_lock<std::mutex> lock(ReplayMutex);
		ReplayStroke = store.Strokes();
		store.Clear();
		app->DrawForReplay();
		for (auto& k : ReplayStroke) {
			Stroke tempStroke = k;
			for (auto pt : tempStroke.points) {
				// ReplayRecordingÀÌ false¸é ¸ØÃã
				if (!ReplayRecording) {
					Pause.wait(lock, [this]() {return ReplayRecording == TRUE;});
				}
				store.ReplayCopyPointToCurrent(pt.x, pt.y);
				app->DrawForReplay();
				Sleep(pt.timestamp);
			}
			store.ReplayCopyTempToStrokes(tempStroke);
			store.ReplayClearCurrent();
		};
		});
}