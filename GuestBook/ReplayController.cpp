#include "ReplayController.h"

void ReplayController::ClickReplay() {
	if (ReplayRecording == TRUE) ReplayRecording = FALSE;
	else ReplayRecording = TRUE;
}

void ReplayController::ReplayWait() {
	///������ �Ͻ�����
}

void ReplayController::StartReplay() {
	if (!ReplayRecording) {
		return;
	}
	
	std::lock_guard<std::mutex> lock(ReplayMutex);
	ReplayStroke = store.Strokes();
	store.Clear();
	app -> DrawForReplay();
	ReplayThread = std::thread([&]() {
		///�����Լ��� �������� ���� ���� ���� ���ְ�, �׸��� �Լ� �־��ְ� if �׷��������� ������ ���ͻ��� �׸��� = ������
		///�׸��� ���÷��� ����, if �ƹ��͵� ������ �׸��� x
		/// �׸��� ��ž��ư ������ ���� �ٽ� ������ ����
			for (auto &k : ReplayStroke) {
				if (!ReplayRecording) {
					ReplayWait();
				}
				///��Ʈ��ũ�� �ϳ��� �ִ� �۾�
				app -> DrawForReplay();
			}
		});
}