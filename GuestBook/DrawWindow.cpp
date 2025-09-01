#include "DrawWindow.h"
#include "MainWindow.h"
#include "UIDrawBridge.h"
#include "ToolState.h"

#define DM_REBUILD (WM_APP + 1)

static inline void GetCurrentStyle(const UIDrawBridge* b, int& w, COLORREF& c) {
	w = 2; c = RGB(0, 0, 0);
	if (!b) return;
	w = b->tool.Thickness();
	bool erasing = (b->tool.Tool() == ToolType::Eraser);
	c = erasing ? RGB(255, 255, 255) : b->tool.Color();
}

bool DrawWindow::Create(HWND parentHwnd, HINSTANCE hInst) {
	hInstance = hInst;

	WNDCLASS wc = {};
	wc.lpfnWndProc = DrawWindow::WndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"DrawWindowClass";
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0, wc.lpszClassName, L"Guest Book",
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, ///형제끼리 서로 영역을 덮지 않도록
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
			
			/// 저장된 선들만 1회 렌더, penWidth/color에 전달
			int penW;
			COLORREF penC;
			GetCurrentStyle(bridge, penW, penC);
			controller.DrawStrokes(back->dc(), store.Strokes(), nullptr, penW, penC);
		}
		InvalidateRect(hwnd, nullptr, FALSE);
		return 0;
	}


	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		///RECT rc; GetClientRect(hwnd, &rc);
		OnPaint(hdc, ps.rcPaint);
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

	/// 현재 스타일
	int penW; 
	COLORREF penC; 
	GetCurrentStyle(bridge, penW, penC);

	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;


	/// 버퍼 없으면 직접 그리기 -> 안전장치
	if (!back || !back->dc() || !front || !front->dc() || w <= 0 || h <= 0) {
		FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
		controller.DrawStrokes(hdc, store.Strokes(), store.Current(), penW, penC);
		return;
	}
	BitBlt(front->dc(), rc.left, rc.top, w, h, back->dc(), rc.left, rc.top, SRCCOPY);

	/// 더러워진 영역만 front <- back
	int saved = SaveDC(front->dc());
	IntersectClipRect(front->dc(), rc.left, rc.top, rc.right, rc.bottom);
	static const std::vector<Stroke> empty;
	controller.DrawStrokes(front->dc(), empty, store.Current(), penW, penC);
	RestoreDC(front->dc(), saved);

	// 3) front -> 화면 (더러워진 영역만 복사)
	BitBlt(hdc, rc.left, rc.top, w, h, front->dc(), rc.left, rc.top, SRCCOPY);
}

void DrawWindow::OnLButtonDown(int x, int y, WPARAM) {
	SetCapture(hwnd);
	store.Begin(x, y);

	if (bridge) {
		bool erasing = (bridge->tool.Tool() == ToolType::Eraser);
		COLORREF c = erasing ?  RGB(255, 255, 255) :  bridge->tool.Color();
		int t = bridge->tool.Thickness();

		store.SetCurrentStyle(c, t);
	}
	InvalidateRect(hwnd, nullptr, FALSE);
}

void DrawWindow::OnMouseMove(int x, int y, WPARAM flags) {
	if (store.IsRecording() && (flags & MK_LBUTTON)) {
		// 더티 사각형 계산 (이전 점과 현재 점을 감싸기)
		int t = bridge ? bridge->tool.Thickness() : 2;
		int pad = t + 2;

		RECT dirty = { x - pad, y - pad, x + pad, y + pad };
		if (const Stroke* cur = store.Current()) {
			if (!cur->points.empty()) {
				const Point& p = cur->points.back();
				dirty.left = min(dirty.left, p.x - pad);
				dirty.top = min(dirty.top, p.y - pad);
				dirty.right = max(dirty.right, p.x + pad);
				dirty.bottom = max(dirty.bottom, p.y + pad);
			}
		}

		store.Add(x, y);
		InvalidateRect(hwnd, &dirty, FALSE);   // ← 창 전체가 아니라 작은 영역만
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

			/// 현재 선 스타일 구하여 bridge에 전달하기
			int penW;
			COLORREF penC;
			GetCurrentStyle(bridge, penW, penC);

			/// penWidth/color 전달하기
			controller.DrawStrokes(back->dc(), empty, cur, penW, penC);
		}
	}

	store.End();
	ReleaseCapture();
	InvalidateRect(hwnd, nullptr, FALSE);
}