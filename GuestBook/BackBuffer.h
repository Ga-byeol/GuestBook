#pragma once
#include <windows.h>

class BackBuffer {
public:
	~BackBuffer() { ReleaseBuffer(); }

	void CreateBuffer(HDC refDC, int w, int h);
	void ClearBuffer(const RECT& rc) const;
	void DrawBufferToScreen(HDC dst, int x = 0, int y = 0) const;
	HDC dc() const { return memdc;  }
	void ReleaseBuffer();

private:
	HDC memdc = nullptr; // 메모리 DC
	HBITMAP bmp = nullptr; // 백버퍼 비트맵
	HGDIOBJ old = nullptr; // Select 이전 객체
	int w = 0;
	int h = 0;
};

inline void BackBuffer::CreateBuffer(HDC refDC, int w, int h) {
	if (memdc && bmp && w == w && h == h) return; // 맞으면 그대로
	ReleaseBuffer(); 
	memdc = CreateCompatibleDC(refDC);
	bmp = CreateCompatibleBitmap(refDC, w, h);
	old = SelectObject(memdc, bmp);
	w = w; h = h;
}

inline void BackBuffer::ClearBuffer(const RECT& rc) const {
	if (!memdc) return;
	FillRect(memdc, &rc, (HBRUSH)(COLOR_WINDOW + 1)); // 윈도우 기본 배경
}

inline void BackBuffer::DrawBufferToScreen(HDC dst, int x, int y) const {
	if (!memdc) return;
	BitBlt(dst, x, y, w, h, memdc, 0, 0, SRCCOPY);
}

inline void BackBuffer::ReleaseBuffer() {
	if (memdc) {
		if (old) SelectObject(memdc, old);
		if (bmp) DeleteObject(bmp);
		DeleteDC(memdc);
	}
	memdc = nullptr; bmp = nullptr; old = nullptr;
	w = h = 0;
}