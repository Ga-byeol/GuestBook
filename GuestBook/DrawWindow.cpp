#include "DrawWindow.h"

bool DrawWindow::Create(HWND parentHwnd, HINSTANCE hInst) {
	hInstance = hInst;

	WNDCLASS wc = {};
	wc.lpfnWndProc = DrawWindow::WndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"DrawWindowClass";
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0, wc.lpszClassName, L"Guest Book",
		WS_CHILD | WS_VISIBLE,
		0, 50, 800, 600,
		parentHwnd, NULL, hInst, this);

	return hwnd != nullptr;
}

LRESULT CALLBACK DrawWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	DrawWindow* self = nullptr;

	if (msg == WM_NCCREATE) {
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		self = reinterpret_cast<DrawWindow*>(cs->lpCreateParams);
		if (self) {
			self->hwnd = hwnd;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
		}
		return TRUE;
	}

	self = reinterpret_cast<DrawWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (self) return self->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT DrawWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rc; GetClientRect(hwnd, &rc);
		OnPaint(hdc, rc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_LBUTTONDOWN:
		OnLButtonDown((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), wParam);
		return 0;

	case WM_MOUSEMOVE:
		OnMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), wParam);
		return 0;

	case WM_LBUTTONUP:
		OnLButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), wParam);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}



void DrawWindow::OnPaint(HDC hdc, const RECT&) {

	controller.DrawAllStrokesWithCurrent(hdc, store.Strokes(), store.Current());
}

void DrawWindow::OnLButtonDown(int x, int y, WPARAM) {
	SetCapture(hwnd);
	store.Begin(x, y);
	InvalidateRect(hwnd, nullptr, FALSE);
}

void DrawWindow::OnMouseMove(int x, int y, WPARAM flags) {
	if (store.IsRecording() && (flags & MK_LBUTTON)) {
		store.Add(x, y);
		InvalidateRect(hwnd, nullptr, FALSE);
	}
}

void DrawWindow::OnLButtonUp(int x, int y, WPARAM) {
	if (!store.IsRecording()) return;
	store.Add(x, y);
	store.End();
	ReleaseCapture();
	InvalidateRect(hwnd, nullptr, FALSE);
}
