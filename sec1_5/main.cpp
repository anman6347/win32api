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
        EndDialog(hDlg, TRUE);
        return TRUE;
    }
    return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR lpCmdLine, int nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(MENU), HWND_DESKTOP, (DLGPROC)dlgProc);

    return 0;
}
