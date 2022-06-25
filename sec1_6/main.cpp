#include <windows.h>
#include "resource.h"


BOOL CALLBACK dlgProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{

    switch (uMessage) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_TEST:
            SetDlgItemText(hDlg, IDC_COUNT, L"留年");
            return TRUE;

        case IDC_CLOSE:
            DestroyWindow(hDlg);
            return TRUE;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR lpCmdLine, int nCmdShow)
{
    HWND hDlg;
    MSG msg;

    hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(MENU), HWND_DESKTOP, (DLGPROC)dlgProc);

    if (hDlg == 0) {
        return FALSE;
    }
    
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        if (!IsDialogMessage(hDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}
