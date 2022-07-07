#include <windows.h>
#include "resource.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static wchar_t szMsg[256];
    RECT rect;
    WINDOWINFO winInfo;
    int nYstart = 0;

    switch (uMessage) {
    case WM_PAINT:
        BeginPaint(hWnd, &ps);

        GetWindowRect(hWnd, &rect);
        wsprintf(szMsg, L"ウィンドウの位置 = (%d, %d) - (%d, %d)",
                 rect.left,
                 rect.top,
                 rect.right,
                 rect.bottom);
        TextOut(ps.hdc, 10, nYstart += 30, szMsg, wcslen(szMsg));


        winInfo.cbSize = sizeof(WINDOWINFO);
        GetWindowInfo(hWnd, &winInfo);

        wsprintf(szMsg, L"ウィンドウの位置 = (%d, %d) - (%d, %d)",
                 winInfo.rcWindow.left,
                 winInfo.rcWindow.top,
                 winInfo.rcWindow.right,
                 winInfo.rcWindow.bottom);
        TextOut(ps.hdc, 10, nYstart += 30, szMsg, wcslen(szMsg));

        wsprintf(szMsg, L"クライアントウィンドウの位置 = (%d, %d) - (%d, %d)",
                 winInfo.rcClient.left,
                 winInfo.rcClient.top,
                 winInfo.rcClient.right,
                 winInfo.rcClient.bottom);
        TextOut(ps.hdc, 20, nYstart += 30, szMsg, wcslen(szMsg));

        wsprintf(szMsg, L"ウィンドウXボーダー = %d", winInfo.cxWindowBorders ) ;
        TextOut(ps.hdc, 30, nYstart += 30, szMsg, wcslen(szMsg));

        wsprintf(szMsg, L"ウィンドウYボーダー = %d", winInfo.cyWindowBorders);
        TextOut(ps.hdc, 40, nYstart += 30, szMsg, wcslen(szMsg));

        wsprintf( szMsg, L"タイトルバーの高さ = %d", GetSystemMetrics(SM_CYCAPTION));
        TextOut(ps.hdc, 50, nYstart += 30, szMsg, wcslen(szMsg));

        wsprintf( szMsg, L"メニューバーの高さ = %d", GetSystemMetrics(SM_CYMENU));
        TextOut(ps.hdc, 60, nYstart += 30, szMsg, wcslen(szMsg));


        EndPaint(hWnd, &ps);
        break;


    case WM_MOVE:
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return TRUE;
    

    // MENU
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_CLOSE:
            DestroyWindow(hWnd);
            break;
        
        default:
            return DefWindowProc(hWnd, uMessage, wParam, lParam);
        }
        break;
    
    default:
        return DefWindowProc(hWnd, uMessage, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;
    MSG msg;
    HWND hWnd;
    LPCWSTR szTitle = L"ウィンドウ情報";
    LPCWSTR szWindowClass = L"SAMPLE";

    ZeroMemory(&wcx, sizeof(wcx));
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = (WNDPROC)WndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR0));
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcx.lpszMenuName = MAKEINTRESOURCE(WINMENU);
    wcx.lpszClassName = szWindowClass;

    if (!RegisterClassEx(&wcx)) {
        return 0;
    }

    hWnd = CreateWindow(szWindowClass,
                        szTitle,
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        GetSystemMetrics(SM_CXSCREEN) / 2 - 500 / 2,
                        GetSystemMetrics(SM_CYSCREEN) / 2 - 200 / 2,
                        500,
                        300,
                        HWND_DESKTOP,
                        NULL,
                        hInstance,
                        NULL);

    if (!hWnd) {
        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
