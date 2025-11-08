#include "ScreensaverManager.h"
#include <iostream>

ScreensaverManager::ScreensaverManager(HWND hWnd)
    : m_hMainWnd(hWnd), m_isSaverActive(false) {
    // 생성 시점의 시간을 초기 활동 시간으로 설정
    ResetActivityTimer();
}

// 소멸자 (스레드가 실행 중이면 확실히 종료)
ScreensaverManager::~ScreensaverManager() {
    if (m_isSaverActive.load()) {
        StopSaver();
    }
    if (m_drawingThread.joinable()) {
        m_drawingThread.join();
    }
        //m_drawingThread.detach();
    
}

//무활동 검사 (WM_TIMER에서 호출)
void ScreensaverManager::CheckInactivity() {
    // 이미 활성화 상태면 검사 안 함
    if (m_isSaverActive.load()) {
        return;
    }

    ULONGLONG currentTime = GetTickCount64();
    if ((currentTime - m_lastActivityTime) > INACTIVITY_THRESHOLD) {
        // 10초 이상 무활동 시 스크린세이버 시작
        std::cout << "Inactivity detected. Starting saver..." << std::endl;
        StartSaver();
    }
}

//활동 감지 (WndProc에서 호출)
void ScreensaverManager::ResetActivityTimer() {
    //    어떤 경우든 마지막 활동 시간은 현재로 갱신
    m_lastActivityTime = GetTickCount64();
}

// 스크린세이버 활성화 상태 반환
bool ScreensaverManager::IsSaverActive() const {
    return m_isSaverActive.load();
}

// 3. 스크린세이버 시작
void ScreensaverManager::StartSaver() {
    m_isSaverActive = true;

    //스레드 시작
    m_drawingThread = std::thread(&ScreensaverManager::DrawingThreadFunction, this);
}

// 4. 스크린세이버 종료
void ScreensaverManager::StopSaver() {
    if (!m_isSaverActive.load()) {
        return;
    }

    std::cout << "Stopping saver..." << std::endl;
    
    //플래그를 false로 바꿔 스레드의 무한 루프 종료 신호
    m_isSaverActive = false;

    //스레드가 종료될 때까지 대기
    if (m_drawingThread.joinable()) {
        m_drawingThread.join();
    }


    ResetActivityTimer();
}


//그리기 스레드 함수
void ScreensaverManager::DrawingThreadFunction() {
    HDC hdc = GetDC(m_hMainWnd);
    if (!hdc) {
        m_isSaverActive = false; // DC 획득 실패 시 스레드 종료
        return;
    }

        // GDI로 화면 지우기 (검은색)
        RECT clientRect;
        GetClientRect(m_hMainWnd, &clientRect);
        HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &clientRect, hBlackBrush);
        DeleteObject(hBlackBrush);

        // 텍스트 그리기
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkMode(hdc, TRANSPARENT);
        LPCWSTR text = L"스크린세이버 활성화됨";
        DrawText(hdc, text, -1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        ReleaseDC(m_hMainWnd, hdc);
        while(m_isSaverActive.load()){}
    
    std::cout << "Drawing thread finished." << std::endl;
}