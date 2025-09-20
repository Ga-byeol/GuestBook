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
		///pThis->back.ClearBuffer(rc);
		
		/// pThis->drawController.DrawStrokes(pThis->back.dc(), pThis->StrokeController.GetStrokes(), *pThis->StrokeController.Current(), penWidth, color);
		OnPaint(hdc, rc);
		///back.DrawBufferToScreen(hdc);
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


void DrawWindow::OnPaint(HDC hdc, const RECT& rcClient) {
<<<<<<< HEAD
	BackBuffer& back = BackBufferManager::Instance().GetBuffer();
	//back.ClearBuffer(rcClient);
=======
	OutputDebugString(L"OnPaint called\n");
<<<<<<< HEAD
	BackBuffer& back = BackBufferManager::Instance().GetBuffer();
	back.ClearBuffer(rcClient);
>>>>>>> eee0e96 (Feature/replaycontroller (#18))
	controller.DrawStrokes(back.dc(), store.Strokes(), store.Current());
	back.DrawBufferToScreen(hdc);
=======
>>>>>>> e953b4c (feat: BackBuffer::DrawDirtyBufferToScreen êµ¬í˜„, MainWindow::WM_CREATE/WM_SIZE ì‹œì  ë²„í¼ ìƒì„± ë° ìž¬ìƒì„± ì²˜ë¦¬ êµ¬í˜„, DrawWindow ì‹¤ì‹œê°„ ê·¸ë¦¬ê¸° êµ¬í˜„ (ì§„í–‰ ì¤‘))

	BackBuffer& back = mainWindow->Back();

	back.ClearBuffer(rcClient);
	drawCtrl.DrawStrokes(back.dc(), strokeCtrl.Strokes(), strokeCtrl.Current() ? *strokeCtrl.Current() : Stroke(), penWidth, selectedColor); 
	
	back.DrawBufferToScreen(hdc);
}

void DrawWindow::OnLButtonDown(int x, int y, WPARAM) {
	SetCapture(hwnd);
	strokeCtrl.Begin(x, y);
	///InvalidateRect(hwnd, nullptr, FALSE);
}

void DrawWindow::OnMouseMove(int x, int y, WPARAM flags) {
	if (flags & MK_LBUTTON) {
		strokeCtrl.Add(x, y);

		const Stroke* cur = strokeCtrl.Current();
		if (cur && cur->points.size() >= 2) {

			/// ¸¶Áö¸· ¼±¸¸ ¹öÆÛ¿¡ µ¡±×¸®±â
			BackBuffer& back = mainWindow->Back();
			drawCtrl.DrawLatestStroke(back.dc(), *cur, penWidth, selectedColor);

			/// dirty rect °è»ê (µÎ Á¡ »çÀÌ ¿µ¿ª)
			const Point& p1 = cur->points[cur->points.size() - 2];
			const Point& p2 = cur->points.back();
			RECT dirty = { min(p1.x, p2.x) - penWidth,
						   min(p1.y, p2.y) - penWidth,
						   max(p1.x, p2.x) + penWidth,
						   max(p1.y, p2.y) + penWidth };

			/// dirty ¿µ¿ª¸¸ È­¸é¿¡ º¹»çÇÏ±â
			HDC hdc = GetDC(hwnd);
			back.DrawDirtyBufferToScreen(hdc, dirty);
			ReleaseDC(hwnd, hdc);
			///if (store.IsRecording() && (flags & MK_LBUTTON)) {
			///	store.Add(x, y);
			///	InvalidateRect(hwnd, nullptr, FALSE);
		}
	}
}

void DrawWindow::OnLButtonUp(int x, int y, WPARAM) {
	if (!strokeCtrl.IsRecording()) return;

	strokeCtrl.Add(x, y); 
	strokeCtrl.End();  
	ReleaseCapture();

	// ÀüÃ¼ ´Ù½Ã ±×¸®±â ¿¹¾à
	InvalidateRect(hwnd, nullptr, FALSE);
	
	///if (!store.IsRecording()) return;
	///store.Add(x, y);
	///store.End();
	///ReleaseCapture();
	///InvalidateRect(hwnd, nullptr, FALSE);
}
