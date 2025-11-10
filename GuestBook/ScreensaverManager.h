#pragma once
#include <windows.h>
#include <thread>
#include <atomic>

#define INACTIVITY_THRESHOLD 3000

class ScreensaverManager {
public:

    ScreensaverManager(HWND hWnd);
    ~ScreensaverManager();

    // 무활동 검사 함수
    void CheckInactivity();

    // 활동 감지 시 호출될 함수
    void ResetActivityTimer();

    // 스크린세이버 활성화 상태인지 확인
    bool IsSaverActive() const;
    
    void StopSaver();

private:

    void StartSaver();




    void DrawingThreadFunction();

    HWND m_hMainWnd;
    ULONGLONG m_lastActivityTime; // 마지막 활동 시간 (GetTickCount64)
    RECT m_originalWindowRect;    // 원래 윈도우 크기 보관용

    std::thread m_drawingThread; 
    std::atomic<bool> m_isSaverActive; // 스크린세이버 활성화 플래그
};