#pragma once
#include <windows.h>
#include <thread>
#include <atomic>


#define INACTIVITY_THRESHOLD 3000

class ScreensaverManager {
public:

    ScreensaverManager(HWND hWnd, HWND hParent);
    ~ScreensaverManager();

    // 무활동 검사 함수
    void CheckInactivity();

    // 활동 감지 시 호출될 함수
    void ResetActivityTimer();

    // 스크린세이버 활성화 상태인지 확인
    bool IsSaverActive() const;
    
    void StopSaver();

    void RestoreWindow(HWND m_parentHwnd);

    bool Startsavertime();


private:

    void StartSaver();

    void SetFullscreen();

    void DrawingThreadFunction();

    HWND m_hParentWnd;
    HWND m_hMainWnd;
    ULONGLONG m_lastActivityTime; // 마지막 활동 시간 (GetTickCount64)
    ULONG m_saverStartTime; //세이버 시작 시간

    std::thread m_drawingThread; 
    std::atomic<bool> m_isSaverActive; // 스크린세이버 활성화 플래그

};