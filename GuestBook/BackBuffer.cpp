#include "BackBuffer.h"

BackBuffer::~BackBuffer() {
	ReleaseBuffer();
}

<<<<<<< HEAD
void BackBuffer::CreateBuffer(HDC refDC, RECT rc) {
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
=======
void BackBuffer::CreateBuffer(HDC refDC, int w, int h) {
>>>>>>> eee0e96 (Feature/replaycontroller (#18))
	if (memdc && bmp && width == w && height == h) return; // 맞으면 그대로

	ReleaseBuffer();

	memdc = CreateCompatibleDC(refDC);
	if (!memdc) { return; }

	bmp = CreateCompatibleBitmap(refDC, w, h);
	if (!bmp) {
		DeleteDC(memdc);
		memdc = nullptr;
		return;
	}

	old = SelectObject(memdc, bmp);

	width = w;
	height = h;
<<<<<<< HEAD

	FillRect(memdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
=======
>>>>>>> eee0e96 (Feature/replaycontroller (#18))
}

void BackBuffer::ClearBuffer(const RECT& rc) const {
	if (!memdc) return;
	FillRect(memdc, &rc, (HBRUSH)(COLOR_WINDOW + 1)); // 윈도우 기본 배경
}

/// 리플레이 시 전체 복사
void BackBuffer::DrawBufferToScreen(HDC dst, int x, int y) const {
	if (!memdc || !dst || (width <= 0) || (height <= 0)) return;
	BitBlt(dst, x, y, width, height, memdc, 0, 0, SRCCOPY);
}

/// 실시간 그리기용 부분 복사
void BackBuffer::DrawDirtyBufferToScreen(HDC dst, const RECT& dirty, int x, int y) const {
	if (!memdc || !dst || (width <= 0) || (height <= 0)) return;
	int w = dirty.right - dirty.left;
	int h = dirty.bottom - dirty.top; 
	BitBlt(dst, dirty.left + x, dirty.top + y, w, h, memdc, dirty.left, dirty.top,SRCCOPY);
}


void BackBuffer::ReleaseBuffer() {
	if (memdc) {
		if (old) {
			SelectObject(memdc, old);
			old = nullptr;
		}
		if (bmp) {
			DeleteObject(bmp);
			bmp = nullptr;
		}
		DeleteDC(memdc);
		memdc = nullptr;
	}
	width = height = 0;
}