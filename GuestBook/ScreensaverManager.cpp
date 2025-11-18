#include "ScreensaverManager.h"
#include <iostream>

ScreensaverManager::ScreensaverManager(HWND hWnd, HINSTANCE hInst)
    : m_hMainWnd(hWnd),
    m_hInstance(hInst),      // 인스턴스 저장
    m_hSaverWnd(NULL),       // 세이버 핸들 초기화
    m_isSaverActive(false)
{
    ResetActivityTimer();
    RegisterSaverWndClass(); // 생성 시 세이버 윈도우 클래스 등록
}

ScreensaverManager::~ScreensaverManager() {
    if (m_hSaverWnd) {
        // 매니저가 소멸할 때 세이버 윈도우도 닫음
        DestroyWindow(m_hSaverWnd);
    }
}

void ScreensaverManager::RegisterSaverWndClass()
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = GlobalSaverWndProc; // C-style 함수 연결
    wc.hInstance = m_hInstance;
    wc.hCursor = NULL; // 커서 숨김
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // 검은색 배경
    wc.lpszClassName = SAVER_WND_CLASS_NAME; // #define으로 정의된 이름

    RegisterClassEx(&wc);
}

void ScreensaverManager::ShowSaverWindow()
{
    if (m_hSaverWnd != NULL) {
        return; // 이미 켜져 있음
    }

    m_isSaverActive = true;
    ShowCursor(FALSE); // 마우스 커서 숨기기

    m_saverStartTime = GetTickCount64();

    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);

    m_hSaverWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,            // 항상 최상위
        SAVER_WND_CLASS_NAME,     // 등록한 클래스 이름
        L"Saver Mode",
        WS_POPUP | WS_VISIBLE,    // 팝업 스타일
        0, 0, cxScreen, cyScreen, // 전체 화면 크기
        NULL,                     // 부모 없음 (독립 윈도우)
        NULL,
        m_hInstance,
        this                      // [중요] lpParam에 'this' 포인터 전달
    );
    if (m_hSaverWnd != NULL) {
        OutputDebugString(L"ERROR: Saver Window creation success\n");
    }
    if (m_hSaverWnd) {
        SetWindowPos(m_hSaverWnd, HWND_TOPMOST,
            0, 0, cxScreen, cyScreen,
            SWP_SHOWWINDOW);
        SetForegroundWindow(m_hSaverWnd);
    }

}


LRESULT CALLBACK GlobalSaverWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ScreensaverManager* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<ScreensaverManager*>(cs->lpCreateParams);
        // 'this' 포인터를 윈도우 데이터로 저장
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    // 저장된 'this' 포인터 가져오기
    pThis = reinterpret_cast<ScreensaverManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (pThis) {
        // 멤버 함수로 메시지 전달
        return pThis->HandleSaverMessage(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT ScreensaverManager::HandleSaverMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const ULONGLONG IGNORE_INPUT_DELAY = 1000;

    // 현재 시간과 시작 시간 비교
    ULONGLONG currentTime = GetTickCount64();

    // 1초 미만이라면 입력 이벤트를 무시합니다.
    if ((currentTime - m_saverStartTime) < IGNORE_INPUT_DELAY) {
        // WM_DESTROY만 아니면 모든 입력 메시지를 무시하고 DefWindowProc으로 전달하지 않습니다.
        if (message != WM_DESTROY) return 0;
    }
    switch (message)
    {
        // 어떤 입력이라도 감지되면 윈도우 종료
    case WM_KEYDOWN:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        ShowCursor(TRUE);     // 커서 다시 보이기
        DestroyWindow(hWnd);  // 윈도우 파괴 (WM_DESTROY 호출)
        break;

    case WM_PAINT:
    {
        // 배경색 칠하기를 강제합니다.
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        // 검은색 브러시를 가져와 클라이언트 영역을 채웁니다.
        HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        FillRect(hdc, &clientRect, hBlackBrush);

        EndPaint(hWnd, &ps);
        return 0; // WM_PAINT 처리 완료
    }

    case WM_DESTROY:
        m_hSaverWnd = NULL;       // 핸들 초기화
        m_isSaverActive = false;  // 상태 변경
        ResetActivityTimer();     // 닫힌 시점부터 다시 타이머 시작
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//무활동 검사 (WM_TIMER에서 호출)
void ScreensaverManager::CheckInactivity() {
    // 이미 활성화 상태면 검사 안 함
    if (m_isSaverActive.load()) {
        return;
    }
    // 시스템 전체 유휴 시간 확인 (GetLastInputInfo)
    LASTINPUTINFO lii = { 0 };
    lii.cbSize = sizeof(LASTINPUTINFO);
    DWORD dwSystemIdleTime = 0;
    if (GetLastInputInfo(&lii)) {
        dwSystemIdleTime = (DWORD)(GetTickCount64() - lii.dwTime);
    }
    else {
        dwSystemIdleTime = (DWORD)(GetTickCount64() - m_lastActivityTime);
    }

    // 앱 내부 유휴 시간 확인 (isReplaying 등에 의해 갱신됨)
    ULONGLONG dwAppIdleTime = GetTickCount64() - m_lastActivityTime;

    // 두 조건이 모두 임계값을 넘었는지 확인
    if ((dwSystemIdleTime > INACTIVITY_THRESHOLD) && (dwAppIdleTime > INACTIVITY_THRESHOLD)) {
        std::cout << "Inactivity detected. Starting saver window..." << std::endl;
        ShowSaverWindow(); // StartSaver() 대신 호출
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