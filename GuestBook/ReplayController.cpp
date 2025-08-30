#include "ReplayController.h"

void ReplayController::ClickReplay() {
	if (ReplayRecording == TRUE) ReplayRecording = FALSE;
	else ReplayRecording = TRUE;
}

void ReplayController::ReplayPause() {
	///스레드 일시정지
}

void ReplayController::StartReplay() {
	///그리고 리플레이 실행, if 아무것도 없으면 그리기 x
	if (store.Strokes().empty()) {
		return;
	}
	std::lock_guard<std::mutex> lock(ReplayMutex);
	ReplayStroke = store.Strokes();
	store.Clear();
	app -> DrawForReplay();
	ReplayThread = std::thread([this]() {
		/// 그리고 스탑버튼 누르면 멈춤 다시 누르면 실행
			for (auto &k : ReplayStroke) {
				Stroke tempStroke = k;
				
				/// k의 선 정보를 tempStroke에 넣어줘야해
				/// tempStroke.color = k.color;
				///tempStroke.thickness = k.thickness;
				
				for (auto pt : tempStroke.points) {
					/// 중지명령
					/// current.points.push_back(pt);
					app->DrawForReplay();
					Sleep(pt.timestamp);
				}

				/// strokes.push_back(tempStroke);
				/// current.clear();
				
				//app -> DrawForReplay();
			}
		});
}