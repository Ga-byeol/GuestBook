#include "ButtonController.h"

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

void ButtonController::RegisterHandler(int id, std::function<void()> fn) {
	ButtonMessage[id] = fn;
}

