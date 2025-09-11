#pragma once
#include <windows.h>

class BackBuffer {
public:
	BackBuffer() = default;
	~BackBuffer();

	int width = 0;
	int height = 0;

	void CreateBuffer(HDC refDC, RECT rc);
	void ClearBuffer(const RECT& rc) const;
	void DrawBufferToScreen(HDC dst, int x = 0, int y = 0) const;
	HDC dc() const { return memdc; }
	void ReleaseBuffer();

private:
	HDC memdc = nullptr; // �޸� DC
	HBITMAP bmp = nullptr; // ����� ��Ʈ��
	HGDIOBJ old = nullptr; // Select ���� ��ü
};