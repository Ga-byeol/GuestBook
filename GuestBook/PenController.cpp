#include "PenController.h"
#include "DrawWindow.h"
#include "resource.h";
#include <string>

PenController::PenController(HINSTANCE hInst, HWND parent)
{
    hInstance = hInst;
    parentHwnd = parent;

}

void PenController::ShowDialog()
{
    DialogBoxParam(
        hInstance,
        MAKEINTRESOURCE(IDD_BRUSH_DIALOG),
        parentHwnd,
        DialogMsg,
        reinterpret_cast<LPARAM>(this)
    );
}

INT_PTR CALLBACK PenController::DialogMsg(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
        {
    case WM_INITDIALOG: /// 다이얼로그 생성 시 운영체제가 보내는 메세지
        SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam); /// 메세지를 받으면 컨트롤 초기화, 기본값 세팅, 객체 연결 
        PenController* pThis = reinterpret_cast<PenController*>(lParam);
        CheckRadioButton(hwndDlg, IDC_RADIO_SOLID, IDC_RADIO_DASH, IDC_RADIO_SOLID); /// 다이얼로그 창 생성 시 버튼 초기화 위치

        HWND hCombo = GetDlgItem(hwndDlg, IDC_COMBOX_PENWIDTH); /// 다이얼로그 콤보박스 초기화
        for (int i = 1; i <= 10; ++i) {
            std::wstring str = std::to_wstring(i);
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)str.c_str());
        }
        SendMessage(hCombo, CB_SETCURSEL, 0, 0);
        return TRUE;
    }
    case WM_COMMAND:
    {
        PenController* pThis = reinterpret_cast<PenController*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
        if (!pThis) break;

        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            HWND hCombo = GetDlgItem(hwndDlg, IDC_COMBOX_PENWIDTH);
            int size = (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (size != CB_ERR) {
                pThis->PenWidth = size + 1;  /// 1~10 중 하나 저장
            }


            if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_SOLID)) {  /// 라디오 버튼 체크
                pThis->PenNum = 0;
            }
            else if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_DASH)) {
                pThis->PenNum = 1;
            }
            else if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_DOT)) {
                pThis->PenNum = 2;
            }


            if (pThis->drawWindow) {
                pThis->drawWindow->SetPenStyle(pThis->PenNum);
                pThis->drawWindow->SetPenWidth(pThis->PenWidth);
            }

            EndDialog(hwndDlg, IDOK);
            return TRUE;
        }
        case IDCANCEL:
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
        }
    }
    break;
    }
    return FALSE;
}

