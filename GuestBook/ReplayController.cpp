#include "ReplayController.h"



void ReplayController::StartReplay() {
	if (store.IsRecording()) {
		return;
	}
	
	std::lock_guard<std::mutex> lock(ReplayMutex);
	ReplayStroke = store.Strokes();
	store.Clear();
	//�׸����Լ�ȣ��
	ReplayThread = std::thread([&]() {
		///�����Լ��� �������� ���� ���� ���� ���ְ�, �׸��� �Լ� �־��ְ� if �׷��������� ������ ���ͻ��� �׸��� = ������
		///�׸��� ���÷��� ����, if �ƹ��͵� ������ �׸��� x
		/// �׸��� ��ž��ư ������ ���� �ٽ� ������ ����
			for (auto &k : ReplayStroke) {
				///��Ʈ��ũ�� �ϳ��� �ִ� �۾�
				///�׸��� ȣ��
				
					
			}
		});
}