#pragma once
#include "BackBuffer.h"

class BackBufferManager {
public:
	static BackBufferManager& Instance() {
		static BackBufferManager instance;
		return instance;
	}

	BackBuffer& GetBuffer() { return buffer; }

<<<<<<< HEAD
    void ResizeBuffer(HDC hdc, RECT rc) {
        buffer.CreateBuffer(hdc, rc);
=======
    void ResizeBuffer(HDC hdc, int w, int h) {
        buffer.CreateBuffer(hdc, w, h);
>>>>>>> eee0e96 (Feature/replaycontroller (#18))
    }

private:
    BackBufferManager() = default;
    ~BackBufferManager() = default;

    BackBufferManager(const BackBufferManager&) = delete;
    BackBufferManager& operator=(const BackBufferManager&) = delete;

    BackBuffer buffer;
};