#pragma once
#include <windows.h>
#include "ToolState.h"

/// 전역 변수를 만들지 않고 한 곳에 객체를 만들고 필요한 곳에 전달하여 사용하는 UI&Draw 공유 파일



class StrokeStore;

struct UIDrawBridge {
	/// 그리기 설정 저장 상자	
	/// UI: tool.SetTool / SetColor / SetThickness 저장만
	/// Draw: tool.Tool / Color / Thickness 게터 읽기만
	ToolState tool{};	/// 기본값 (color=RGB(0,0,0), thickness=2, tool=ToolType::Pen) 초기화

	/// StrokeStore 대한 포인터로 포인터만 들고 있어 주소만 주입함
	/// CLEAR: store->Clear()
	StrokeStore* store = nullptr;	/// nullptr로 초기화

	/// 버튼에서 상태 바뀌거나 CLEAR 실행하면 InvalidateRect(hDraw, nullptr, FALSE); 호출
	HWND hDraw = nullptr;	/// nullptr로 초기화
};