#include "DrawWindow.h"
#include "MainWindow.h"

#define DM_REBUILD (WM_APP + 1)

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

	/// 배경 지우기를 ClearBuffer()에서 함(눈 깜빡임 줄이기)
	case WM_ERASEBKGND:
		return 1;

	case DM_REBUILD:
	{
		auto owner = reinterpret_cast<MainWindow*>(
			GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA));
		BackBuffer* back = owner ? owner->GetBackBuffer() : nullptr;
		if (back && back->dc()) {
			RECT rc; GetClientRect(hwnd, &rc);
			
			/// 누적버퍼 초기화
			back->ClearBuffer(rc);                                       
			
			/// 저장된 선들만 1회 렌더
			controller.DrawStrokes(back->dc(), store.Strokes(), nullptr);
		}
		InvalidateRect(hwnd, nullptr, FALSE);
		return 0;
	}


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



void DrawWindow::OnPaint(HDC hdc, const RECT& rc) {
	auto owner = reinterpret_cast<MainWindow*>(GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA));
	BackBuffer* back = owner ? owner->GetBackBuffer() : nullptr;
	BackBuffer* front = owner ? owner->GetFrontBuffer() : nullptr;

	if (!back || !back->dc() || !front || !front->dc()) {
		/// 안전 장치
		controller.DrawStrokes(hdc, store.Strokes(), store.Current());
		return;
	}
	int w = rc.right - rc.left, h = rc.bottom - rc.top;

	/// front <- back 복사하기
	BitBlt(front->dc(), 0, 0, w, h, back->dc(), 0, 0, SRCCOPY);

	/// current만 front에 복사하기
	static const std::vector<Stroke> empty;
	controller.DrawStrokes(front->dc(), empty, store.Current());

	/// front -> 화면DC인 hdc에 BitBlt
	front->DrawBufferToScreen(hdc);
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

	auto owner = reinterpret_cast<MainWindow*>(
		GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA));
	BackBuffer* back = owner ? owner->GetBackBuffer() : nullptr;

	if (back && back->dc()) {
		const Stroke* cur = store.Current();
			if(cur && !cur->points.empty()) {
			static const std::vector<Stroke> empty;
			controller.DrawStrokes(back->dc(), empty, cur);
		}
	}

	store.End();
	ReleaseCapture();
	InvalidateRect(hwnd, nullptr, FALSE);
}