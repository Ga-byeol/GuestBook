#pragma once
#include <windows.h>
#include <atomic>
#include <thread>
#include <vector>
#include <string>
#include "Stroke.h"
#define INACTIVITY_THRESHOLD 60000 //세이버 시간

#define SAVER_WND_CLASS_NAME L"IndependentSaverWindow"

LRESULT CALLBACK GlobalSaverWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class ScreensaverManager {
public:

    ScreensaverManager(HWND hWnd, HINSTANCE hInst);
    ~ScreensaverManager();

    void RegisterSaverWndClass(); // 세이버 윈도우 클래스 등록
    void ShowSaverWindow();       // 세이버 윈도우 생성


    void StartSaverThread();     // 스레드 시작
    void StopSaverThread();      // 스레드 종료
    void SaverLoop();            // 실제 그리기를 수행하는 루프 함수


    // GlobalSaverWndProc에서 호출할 실제 메시지 핸들러
    LRESULT HandleSaverMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // GlobalSaverWndProc이 pThis를 얻을 수 있도록 friend 선언
    friend LRESULT CALLBACK GlobalSaverWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


    // 무활동 검사 함수
    void CheckInactivity();

    // 활동 감지 시 호출될 함수
    void ResetActivityTimer();

    // 스크린세이버 활성화 상태인지 확인
    bool IsSaverActive() const;


private:
    ULONGLONG m_saverStartTime;
    HWND m_hMainWnd;
    HWND m_hSaverWnd;       // (새로운 세이버 윈도우 핸들)
    HINSTANCE m_hInstance;
    ULONGLONG m_lastActivityTime; // 마지막 활동 시간 (GetTickCount64)
    std::atomic<bool> m_isSaverActive; // 스크린세이버 활성화 플래그

    // 스레드 제어
    std::atomic<bool> m_stopThread; // 스레드 종료 플래그
    std::thread m_saverThread;      // 리플레이를 담당할 워커 스레드

    std::vector<std::wstring> GetFileList(const std::wstring& directory);

};