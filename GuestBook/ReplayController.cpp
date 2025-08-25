#include "ReplayController.h"



void ReplayController::StartReplay() {
	if (store.IsRecording()) {
		return;
	}
	
	std::lock_guard<std::mutex> lock(ReplayMutex);
	ReplayStroke = store.Strokes();
	store.Clear();
	//그리기함수호출
	ReplayThread = std::thread([&]() {
		///람다함수에 동작을할 공간 벡터 복사 해주고, 그리기 함수 넣어주고 if 그려져있으면 복사후 벡터삭제 그리기 = 지워짐
		///그리고 리플레이 실행, if 아무것도 없으면 그리기 x
		/// 그리고 스탑버튼 누르면 멈춤 다시 누르면 실행
			for (auto &k : ReplayStroke) {
				///스트로크에 하나씩 넣는 작업
				///그리기 호출
				
					
			}
		});
}