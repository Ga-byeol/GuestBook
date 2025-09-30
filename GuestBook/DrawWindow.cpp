#include "DrawWindow.h"
#include "ColorController.h"
#include "BackBuffer.h"
#include "MainWindow.h"

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
			RECT rc;
			GetClientRect(hwnd, &rc);
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

		case WM_USER_REPLAY_UPDATE:
			InvalidateRect(hwnd, NULL, false);
			UpdateWindow(hwnd);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}


	void DrawWindow::OnPaint(HDC hdc, const RECT & rcClient) {
		if (!backBuffer) return;
	///	backBuffer->ClearBuffer(rcClient);

		///drawController 사용해 전체 stroke 그리기
		drawCtrl.DrawStrokes(backBuffer->dc(), strokeCtrl.Strokes(), strokeCtrl.Current() ? *strokeCtrl.Current() : Stroke(), penWidth, selectedColor);
		
		backBuffer->DrawBufferToScreen(hdc);
	}

	void DrawWindow::OnLButtonDown(int x, int y, WPARAM) {
		SetCapture(hwnd);
		strokeCtrl.Begin(x, y, erasing ? RGB(255, 255, 255) : selectedColor);
	}

	void DrawWindow::OnMouseMove(int x, int y, WPARAM flags) {
		if (!backBuffer) return;
		if (flags & MK_LBUTTON) {
			strokeCtrl.Add(x, y);

			const Stroke* cur = strokeCtrl.Current();
			if (cur && cur->points.size() >= 2) {

				/// 마지막 선만 버퍼에 덧그리기
				drawCtrl.DrawLatestStroke(backBuffer->dc(), *cur, penWidth, erasing ? RGB(255,255,255) : selectedColor);

				/// dirty rect 계산 (두 점 사이 영역)
				const Point& p1 = cur->points[cur->points.size() - 2];
				const Point& p2 = cur->points.back();
				RECT dirty = { min(p1.x, p2.x) - penWidth,
							   min(p1.y, p2.y) - penWidth,
							   max(p1.x, p2.x) + penWidth,
							   max(p1.y, p2.y) + penWidth };

				/// dirty 영역만 화면에 복사하기
				HDC hdc = GetDC(hwnd);
				backBuffer->DrawDirtyBufferToScreen(hdc, dirty);
				ReleaseDC(hwnd, hdc);
			}
		}
	}

	void DrawWindow::OnLButtonUp(int x, int y, WPARAM) {
		if (!backBuffer) return;
		if (strokeCtrl.IsRecording()) {

			strokeCtrl.Add(x, y);
			strokeCtrl.End();
			ReleaseCapture();
		}
		// 전체 다시 그리기 예약
		InvalidateRect(hwnd, nullptr, FALSE);
	}

	void DrawWindow::ClearAll() {
		strokeCtrl.Clear();

		RECT rc;
		GetClientRect(hwnd, &rc);
		if (backBuffer) {
			backBuffer->ClearBuffer(rc);
		}

		InvalidateRect(hwnd, nullptr, TRUE);
	}