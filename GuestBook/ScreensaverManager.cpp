#include "ScreensaverManager.h"
#include <fstream> // 파일 읽기용
#include <string>

// C++14에서는 <filesystem> 대신 WinAPI 사용

ScreensaverManager::ScreensaverManager(HWND hWnd, HINSTANCE hInst)
    : m_hMainWnd(hWnd),
    m_hInstance(hInst),
    m_hSaverWnd(NULL),
    m_isSaverActive(false),
    m_stopThread(false)
{
    ResetActivityTimer();
    RegisterSaverWndClass();
}

ScreensaverManager::~ScreensaverManager() {
    StopSaverThread();
    if (m_hSaverWnd) {
        DestroyWindow(m_hSaverWnd);
    }
}

void ScreensaverManager::RegisterSaverWndClass()
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = GlobalSaverWndProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = NULL;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = SAVER_WND_CLASS_NAME;

    RegisterClassEx(&wc);
}

void ScreensaverManager::ShowSaverWindow()
{
    if (m_hSaverWnd != NULL) return;

    m_isSaverActive = true;
    ShowCursor(FALSE);
    m_saverStartTime = GetTickCount64();

    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);

    m_hSaverWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        SAVER_WND_CLASS_NAME,
        L"Saver Mode",
        WS_POPUP | WS_VISIBLE,
        0, 0, cxScreen, cyScreen,
        NULL, NULL, m_hInstance, this
    );

    if (m_hSaverWnd) {
        SetWindowPos(m_hSaverWnd, HWND_TOPMOST, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);
        SetForegroundWindow(m_hSaverWnd);
        StartSaverThread(); // 스레드 시작
    }
}

// --- [WinAPI] 파일 목록 가져오기 (C++14 호환) ---
std::vector<std::wstring> ScreensaverManager::GetFileList(const std::wstring& directory) {
    std::vector<std::wstring> files;
    std::wstring searchPath = directory + L"*.dat"; // 검색 패턴
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // 디렉토리가 아닌 파일인 경우만 추가
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                files.push_back(directory + fd.cFileName);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return files;
}

void ScreensaverManager::StartSaverThread() {
    StopSaverThread();
    m_stopThread = false;
    m_saverThread = std::thread(&ScreensaverManager::SaverLoop, this);
}

void ScreensaverManager::StopSaverThread() {
    m_stopThread = true;
    if (m_saverThread.joinable()) {
        m_saverThread.join();
    }
}

// --- 메인 로직: 파일 직접 읽기 및 그리기 ---
void ScreensaverManager::SaverLoop() {

    std::wstring dirPath = L"..\\file\\"; // 파일 경로

    while (!m_stopThread) {
        // 1. WinAPI로 파일 목록 가져오기
        std::vector<std::wstring> files = GetFileList(dirPath);

        // 파일이 없으면 잠시 대기
        if (files.empty()) {
            for (int i = 0; i < 20; i++) {
                if (m_stopThread) return;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            continue;
        }

        // 2. 파일 순회
        for (const auto& filePath : files) {
            if (m_stopThread) return;

            // 화면 지우기 (새 그림 시작 전)
            HDC hdc_clear = GetDC(m_hSaverWnd);
            if (hdc_clear) {
                RECT rc;
                GetClientRect(m_hSaverWnd, &rc);
                FillRect(hdc_clear, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
                ReleaseDC(m_hSaverWnd, hdc_clear);
            }

            // -----------------------------------------------------------
            // ★ 파일 직접 열기 및 파싱 (FileManager::Load 로직 이식)
            // -----------------------------------------------------------
            std::vector<Stroke> outStrokes;
            std::wifstream loadFile(filePath);

            if (loadFile.is_open()) {
                size_t strokeCount;
                loadFile >> strokeCount; // 스트로크 개수 읽기

                for (size_t i = 0; i < strokeCount; ++i) {
                    Stroke s;
                    size_t pointCount;
                    // FileManager와 동일한 순서로 데이터 읽기
                    loadFile >> s.color >> s.thickness >> s.penStyle >> s.penWidth >> pointCount;

                    for (size_t j = 0; j < pointCount; ++j) {
                        Point p;
                        loadFile >> p.x >> p.y >> p.timestamp;
                        s.points.push_back(p);
                    }
                    outStrokes.push_back(s);
                }
                loadFile.close();
            }
            else {
                continue; // 파일 열기 실패 시 다음 파일로
            }
            // -----------------------------------------------------------

            // 3. 읽어온 데이터로 그리기 (Replay)
            for (const auto& s : outStrokes) {
                if (m_stopThread) break;

                for (size_t i = 1; i < s.points.size(); ++i) {
                    if (m_stopThread) break;

                    Sleep(s.points[i].timestamp); // 타임스탬프 대기

                    HDC hdc = GetDC(m_hSaverWnd);
                    if (hdc) {
                        SetGraphicsMode(hdc, GM_ADVANCED);
                        LOGBRUSH lb = {};
                        lb.lbStyle = BS_SOLID;
                        lb.lbColor = s.color;

                        HPEN pen = ExtCreatePen(
                            PS_GEOMETRIC | s.penStyle,
                            s.penWidth,
                            &lb, 0, nullptr
                        );

                        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                        MoveToEx(hdc, s.points[i - 1].x, s.points[i - 1].y, nullptr);
                        LineTo(hdc, s.points[i].x, s.points[i].y);

                        SelectObject(hdc, oldPen);
                        DeleteObject(pen);
                        ReleaseDC(m_hSaverWnd, hdc);
                    }
                }
            }

            // 한 그림 완료 후 대기
            for (int i = 0; i < 20; i++) {
                if (m_stopThread) break;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
}

LRESULT CALLBACK GlobalSaverWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ScreensaverManager* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<ScreensaverManager*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    pThis = reinterpret_cast<ScreensaverManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (pThis) {
        return pThis->HandleSaverMessage(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT ScreensaverManager::HandleSaverMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const ULONGLONG IGNORE_INPUT_DELAY = 1000;
    ULONGLONG currentTime = GetTickCount64();

    if ((currentTime - m_saverStartTime) < IGNORE_INPUT_DELAY) {
        if (message != WM_DESTROY) return 0;
    }

    switch (message)
    {
    case WM_KEYDOWN:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        ShowCursor(TRUE);
        DestroyWindow(hWnd);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        StopSaverThread(); // 종료 시 스레드 중단
        m_hSaverWnd = NULL;
        m_isSaverActive = false;
        ResetActivityTimer();
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void ScreensaverManager::CheckInactivity() {
    if (m_isSaverActive.load()) return;

    LASTINPUTINFO lii = { 0 };
    lii.cbSize = sizeof(LASTINPUTINFO);
    DWORD dwSystemIdleTime = 0;

    if (GetLastInputInfo(&lii)) {
        dwSystemIdleTime = (DWORD)(GetTickCount64() - lii.dwTime);
    }
    else {
        dwSystemIdleTime = (DWORD)(GetTickCount64() - m_lastActivityTime);
    }

    ULONGLONG dwAppIdleTime = GetTickCount64() - m_lastActivityTime;

    if ((dwSystemIdleTime > INACTIVITY_THRESHOLD) && (dwAppIdleTime > INACTIVITY_THRESHOLD)) {
        ShowSaverWindow();
    }
}

void ScreensaverManager::ResetActivityTimer() {
    m_lastActivityTime = GetTickCount64();
}

bool ScreensaverManager::IsSaverActive() const {
    return m_isSaverActive.load();
}