#include "DrawWindow.h"
#include "ColorController.h"
#include "BackBuffer.h"
#include "MainWindow.h"
#include "Application.h"

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
			0, 50, 700, 600,
			parentHwnd, NULL, hInst, this);
		OutputDebugString(L"create drawWindow\n");
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
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
		self = reinterpret_cast<DrawWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (self) return self->HandleMessage(msg, wParam, lParam);

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	LRESULT DrawWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_CREATE: {
			// ... (커서 로드 등)

			// ★ (중요) WM_MOUSELEAVE 이벤트를 받도록 등록
			TRACKMOUSEEVENT tme = { 0 };
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hwnd;
			TrackMouseEvent(&tme);
			return 0;
		}
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
		// ★★★ 1. 커서 숨기기 ★★★
        case WM_SETCURSOR: {
			if (LOWORD(lParam) == HTCLIENT) {
				HideSystemCursor(); // (우리가 직접 그릴 것이므로)
				return TRUE; // "내가 처리했음"
			}
			else {
				ShowSystemCursor(); // (테두리, 타이틀바 등)
			}
			break; // -> DefWindowProc
		}

						 // ★★★ 2. 커서 복원 ★★★
		case WM_MOUSELEAVE: {
			// 2. ★ (수정) '가짜 커서'가 그려지지 않도록 좌표를 치움
			m_currentMousePos = { -1, -1 }; // (예: 화면 밖)

			// 3. '가짜 커서'가 사라진 화면으로 갱신
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		}
		case WM_SIZE: {
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			OnSize(width, height); // OnSize 헬퍼 함수 호출
			return 0;
		}
		case WM_DESTROY: {
			if (backBuffer) delete backBuffer;
			if (cacheBuffer) delete cacheBuffer;
			backBuffer = nullptr;
			cacheBuffer = nullptr;
			return 0;
		}
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}


	void DrawWindow::OnPaint(HDC hdc, const RECT& rcClient) {
		OutputDebugString(L"called paint drawWindow\n");

		// 0. 버퍼 2개 확인
		if (!backBuffer || !cacheBuffer) return;

		// --- 1. '캐시' 재구축 (완성된 선들) ---
		cacheBuffer->ClearBuffer(rcClient);
		const auto& strokes = strokeCtrl.Strokes();
		drawCtrl.DrawStrokes(cacheBuffer->dc(), strokes, penWidth, selectedColor);

		// --- 2. '백 버퍼' 재구성 ---
		// 2a. '캐시'의 내용을 '백 버퍼'로 복사
		BitBlt(backBuffer->dc(), 0, 0, rcClient.right, rcClient.bottom,
			cacheBuffer->dc(), 0, 0, SRCCOPY);

		// 2b. ★ (수정) '현재 획'을 백 버퍼에 덧그리기 (필수!)
		if (strokeCtrl.IsRecording()) {
			const Stroke* cur = strokeCtrl.Current();
			if (cur) {
				drawCtrl.DrawLatestStroke(backBuffer->dc(), *cur, currentPenWidth,
					erasing ? RGB(255, 255, 255) : selectedColor);
			}
		}

		// 2c. ★ (수정) '가짜 커서'를 백 버퍼에 덧그리기 (필수!)
		drawCtrl.DrawCursorDot(backBuffer->dc(), m_currentMousePos,
			currentPenWidth,
			erasing ? RGB(255, 255, 255) : selectedColor,
			erasing);

		// --- 3. 최종본(백 버퍼)을 스크린으로 전송 ---
		backBuffer->DrawBufferToScreen(hdc);
	}

	void DrawWindow::OnLButtonDown(int x, int y, WPARAM) {
		if (isReplaying) return;
		SetCapture(hwnd);
		COLORREF color = erasing ? RGB(255, 255, 255) : selectedColor;

		strokeCtrl.Begin(x, y, color, currentPenStyle, currentPenWidth);
}

	void DrawWindow::OnMouseMove(int x, int y, WPARAM flags) {
		if (!backBuffer || !cacheBuffer || isReplaying) return;

		// 1. 마우스 위치 저장 (OK)
		m_currentMousePos.x = x;
		m_currentMousePos.y = y;

		// --- 2. 백 버퍼 초기화 (캐시 복사) ---
		// (이것이 찌꺼기를 지우는 가장 빠른 방법)
		RECT rc;
		GetClientRect(hwnd, &rc);
		BitBlt(backBuffer->dc(), 0, 0, rc.right - rc.left, rc.bottom - rc.top,
			cacheBuffer->dc(), 0, 0, SRCCOPY);

		// 3. '그리는 중'일 때 '현재 획' 덧그리기
		if (flags & MK_LBUTTON) {
			strokeCtrl.Add(x, y);

			const Stroke* cur = strokeCtrl.Current();
			if (cur) {
				// (DrawLatestStroke가 '현재 획 전체'를 그려야 '각진 선' 문제가 해결됨)
				drawCtrl.DrawLatestStroke(backBuffer->dc(), *cur, currentPenWidth,
					selectedColor);
			}
		}

		// 4. '가짜 커서(원)' 덧그리기 (OK)
		drawCtrl.DrawCursorDot(backBuffer->dc(), m_currentMousePos,
			currentPenWidth,
			erasing ? RGB(255, 255, 255) : selectedColor,
			erasing);

		// 5. '백 버퍼'를 '스크린'으로 전송
		// (Dirty Rect보다 전체 갱신이 이 구조에서는 더 간단하고 안정적)
		HDC hdc = GetDC(hwnd);
		backBuffer->DrawBufferToScreen(hdc);
		ReleaseDC(hwnd, hdc);
  }

	void DrawWindow::OnLButtonUp(int x, int y, WPARAM) {
		if (!backBuffer || !cacheBuffer || isReplaying) return;

		if (strokeCtrl.IsRecording()) {
			strokeCtrl.Add(x, y);
			const Stroke* finishedStroke = strokeCtrl.Current(); // End() 전에 받아둠

			// ★★★ 1. '완성된 획'을 '캐시'에 덧그려 확정 ★★★
			if (finishedStroke) {
				drawCtrl.DrawLatestStroke(cacheBuffer->dc(), *finishedStroke,
					finishedStroke->penWidth,
					selectedColor);
			}

			strokeCtrl.End();
			ReleaseCapture();

			RECT rc;
			GetClientRect(hwnd, &rc);
			BitBlt(backBuffer->dc(), 0, 0, rc.right - rc.left, rc.bottom - rc.top,
				cacheBuffer->dc(), 0, 0, SRCCOPY);

			// 5b. 'backBuffer' 위에 '소프트 커서'를 다시 그림
			drawCtrl.DrawCursorDot(backBuffer->dc(), m_currentMousePos,
				currentPenWidth,
				selectedColor,
				erasing);

			// 5c. 'backBuffer'의 최종본을 '스크린'으로 전송
			HDC hdc = GetDC(hwnd);
			backBuffer->DrawBufferToScreen(hdc);
			ReleaseDC(hwnd, hdc);
		}
	}

	void DrawWindow::ClearAll() {
		strokeCtrl.Clear();

		RECT rc;
		GetClientRect(hwnd, &rc);
		if (backBuffer) {
			backBuffer->ClearBuffer(rc);
		}
		if (cacheBuffer) { // ★★★ cacheBuffer도 클리어 ★★★
			cacheBuffer->ClearBuffer(rc);
		}

		InvalidateRect(hwnd, nullptr, TRUE); // (OnPaint가 빈 캐시를 그리도록 함)
	}

	HDC DrawWindow::GetMemDc() const { return backBuffer ? backBuffer->dc() : nullptr; }

	void DrawWindow::SetPenStyle(int PenNum) { /// ���̾�α� ���� ��ư �ѹ����� �� ����
   		switch (PenNum) {
		case 0: currentPenStyle = PS_SOLID; break;
		case 1: currentPenStyle = PS_DASH;  break;
		case 2: currentPenStyle = PS_DOT;   break;
		default: currentPenStyle = PS_SOLID; break;
		}


	}
	void DrawWindow::SetPenWidth(int PenWidth) {
		currentPenWidth = PenWidth;
	}

	void DrawWindow::HideSystemCursor() {
		if (!m_isCursorHidden) {
			ShowCursor(FALSE); // '진짜' 커서 숨기기
			m_isCursorHidden = true;
		}
	}

	void DrawWindow::ShowSystemCursor() {
		if (m_isCursorHidden) {
			ShowCursor(TRUE); // '진짜' 커서 다시 표시
			m_isCursorHidden = false;
		}
	}
	// ★★★ (추가) DrawWindow::OnSize 헬퍼 함수 구현 ★★★
	void DrawWindow::OnSize(int width, int height) {
		// 1. 기존 버퍼 삭제
		if (backBuffer) delete backBuffer;
		if (cacheBuffer) delete cacheBuffer;

		if (width == 0 || height == 0) return; // 창이 최소화될 때

		// 2. '자신'의 HDC로 '자신'의 크기에 맞는 버퍼 2개 생성
		HDC hdc = GetDC(hwnd); // 'DrawWindow'의 hwnd

		backBuffer = new BackBuffer();
		backBuffer->CreateBuffer(hdc, width, height);

		cacheBuffer = new BackBuffer();
		cacheBuffer->CreateBuffer(hdc, width, height);

		ReleaseDC(hwnd, hdc);

		// 3. 새 버퍼로 화면 전체를 다시 그리도록 강제
		InvalidateRect(hwnd, NULL, TRUE);
	}