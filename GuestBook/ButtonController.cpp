#define SAVE 101
#define LOAD 102
#define REPLAY 103
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107

#include "ButtonController.h"
#include "UIDrawBridge.h" 
#include "ToolState.h"     
#include "StrokeStore.h"
#define DM_REBUILD (WM_APP + 1)
using namespace std;

void ButtonController::Create(HWND parents, LPCWSTR text, int id, int x, int y, int w, int h)
{
	hwnd = CreateWindowW(
		L"BUTTON",
		text,
		WS_VISIBLE | WS_CHILD,
		x, y,
		w, h,
		parents,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(parents, GWLP_HINSTANCE),
		nullptr
	);
}
void ButtonController::HandleCommand(WPARAM wParam) {
	int id = LOWORD(wParam);
		if (ButtonMessage.count(id)) {
			ButtonMessage[id]();
		}
	}


void ButtonController::MessageOption() {
	auto getBridge = [this]() -> UIDrawBridge* {
		HWND cur = hParent ? hParent : (hwnd ? GetParent(hwnd) : nullptr);
		while (cur) {
			if (auto p = reinterpret_cast<UIDrawBridge*>(GetProp(cur, L"APP_BRIDGE")))
				return p;
			cur = GetParent(cur);
		}
		return nullptr;
	};

	auto invalidate = [getBridge]() {
		if (auto b = getBridge()) {
			if (b->hDraw) InvalidateRect(b->hDraw, nullptr, FALSE);
		}
	};

	ButtonMessage[CLEAR] = [getBridge, invalidate]() {
		if (auto b = getBridge()) {
			if (b->store) {
				b->store->Clear();
				if (b->hDraw) {
					SendMessage(b->hDraw, DM_REBUILD, 0, 0);
					InvalidateRect(b->hDraw, nullptr, FALSE);
				}
			}
		}
	};
	ButtonMessage[ERASE] = [getBridge]() {
		if (auto b = getBridge()){
			b->tool.SetTool(ToolType::Eraser); 
			b->tool.SetThickness(12); 
		}
	};
	ButtonMessage[BRUSH] = [getBridge]() {
		if (auto b = getBridge()) {
			b->tool.SetTool(ToolType::Pen);
			b->tool.SetThickness(2);
		}
	};
	ButtonMessage[COLOR] = [getBridge]() {
		if (auto b = getBridge()) {
			static bool red = false;
			red = !red;
			b->tool.SetColor(red ? RGB(255, 0, 0) : RGB(0, 0, 0));
			if (b->hDraw) InvalidateRect(b->hDraw, nullptr, FALSE);
		}
	};

	ButtonMessage[SAVE] = []() { MessageBox(nullptr, L"저장 버튼", L"TOOL창", MB_OK); };
	ButtonMessage[LOAD] = []() { MessageBox(nullptr, L"불러오기 버튼", L"TOOL창", MB_OK); };
	ButtonMessage[REPLAY] = []() { MessageBox(nullptr, L"리플레이 버튼", L"TOOL창", MB_OK); };
	
	
/*	ButtonMessage[CLEAR] = []() { MessageBox(nullptr, L"전체 지우기 버튼", L"TOOL창", MB_OK); };
	ButtonMessage[ERASE] = []() { MessageBox(nullptr, L"지우기 버튼", L"TOOL창", MB_OK); };
	ButtonMessage[BRUSH] = []() { MessageBox(nullptr, L"브러쉬 버튼", L"TOOL창", MB_OK); };
	ButtonMessage[COLOR] = []() { MessageBox(nullptr, L"색상 버튼", L"TOOL창", MB_OK); }; */

}
