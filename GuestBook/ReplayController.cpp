#include "ReplayController.h"

void ReplayController::ClickReplay() {
	if (ReplayRecording == TRUE) ReplayRecording = FALSE;
	else ReplayRecording = TRUE;
}

void ReplayController::ReplayPause() {
	///������ �Ͻ�����
}

void ReplayController::StartReplay() {
	///�׸��� ���÷��� ����, if �ƹ��͵� ������ �׸��� x
	if (store.Strokes().empty()) {
		return;
	}
	std::lock_guard<std::mutex> lock(ReplayMutex);
	ReplayStroke = store.Strokes();
	store.Clear();
	app -> DrawForReplay();
	ReplayThread = std::thread([this]() {
		/// �׸��� ��ž��ư ������ ���� �ٽ� ������ ����
			for (auto &k : ReplayStroke) {
				Stroke tempStroke = k;
				
				/// k�� �� ������ tempStroke�� �־������
				/// tempStroke.color = k.color;
				///tempStroke.thickness = k.thickness;
				
				for (auto pt : tempStroke.points) {
					/// �������
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