#define SAVE 101
#define LOAD 102
#define REPLAY 103
#define CLEAR 104
#define ERASE 105
#define BRUSH 106
#define COLOR 107
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


void ButtonController::MessageOption() {
	ButtonMessage[SAVE] = []() { MessageBox(nullptr, L"���� ��ư", L"TOOLâ", MB_OK); };
	ButtonMessage[LOAD] = []() { MessageBox(nullptr, L"�ҷ����� ��ư", L"TOOLâ", MB_OK); };
	ButtonMessage[REPLAY] = []() { MessageBox(nullptr, L"���÷��� ��ư", L"TOOLâ", MB_OK); };
	ButtonMessage[CLEAR] = []() { MessageBox(nullptr, L"��ü ����� ��ư", L"TOOLâ", MB_OK); };
	ButtonMessage[ERASE] = []() { MessageBox(nullptr, L"����� ��ư", L"TOOLâ", MB_OK); };
	ButtonMessage[BRUSH] = []() { MessageBox(nullptr, L"�귯�� ��ư", L"TOOLâ", MB_OK); };
	ButtonMessage[COLOR] = []() { MessageBox(nullptr, L"���� ��ư", L"TOOLâ", MB_OK); };

}
