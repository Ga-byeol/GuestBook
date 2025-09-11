#pragma once
#include "BackBuffer.h"

class BackBufferManager {
public:
	static BackBufferManager& Instance() {
		static BackBufferManager instance;
		return instance;
	}

	BackBuffer& GetBuffer() { return buffer; }

    void ResizeBuffer(HDC hdc, RECT rc) {
        buffer.CreateBuffer(hdc, rc);
    }

private:
    BackBufferManager() = default;
    ~BackBufferManager() = default;

    BackBufferManager(const BackBufferManager&) = delete;
    BackBufferManager& operator=(const BackBufferManager&) = delete;

    BackBuffer buffer;
};