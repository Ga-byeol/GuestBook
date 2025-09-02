#include "CanvasWindow.h"

bool CanvasWindow::Create(HWND parentHwnd, HINSTANCE hInst) {
	hInstance = hInst;

	WNDCLASS wc = {};
	wc.lpfnWndProc = CanvasWindow::WndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"CanvasWindowClass";
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

LRESULT CALLBACK CanvasWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	CanvasWindow* self = nullptr;

	if (msg == WM_NCCREATE) {
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		self = reinterpret_cast<CanvasWindow*>(cs->lpCreateParams);
		if (self) {
			self->hwnd = hwnd;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
		}
		return TRUE;
	}

	self = reinterpret_cast<CanvasWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (self) return self->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CanvasWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
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

	case WM_USER_UPDATE:
		InvalidateRect(hwnd, nullptr, FALSE);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}



void CanvasWindow::OnPaint(HDC hdc, const RECT&) {

	drawController.DrawStrokes(hdc, strokeController.Strokes(), strokeController.Current());
}

void CanvasWindow::OnLButtonDown(int x, int y, WPARAM) {
	SetCapture(hwnd);
	strokeController.Begin(x, y);
	InvalidateRect(hwnd, nullptr, FALSE);
}

void CanvasWindow::OnMouseMove(int x, int y, WPARAM flags) {
	if (strokeController.IsRecording() && (flags & MK_LBUTTON)) {
		strokeController.Add(x, y);
		InvalidateRect(hwnd, nullptr, FALSE);
	}
}

void CanvasWindow::OnLButtonUp(int x, int y, WPARAM) {
	if (!strokeController.IsRecording()) return;
	strokeController.Add(x, y);
	strokeController.End();
	ReleaseCapture();
	InvalidateRect(hwnd, nullptr, FALSE);
}
