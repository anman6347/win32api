#include <windows.h>
#include "resource.h"

#define ID_TIMER 200


const int window_width = 300;
const int window_height = 360;

LRESULT CALLBACK WndProc(HWND hWnd, UINT umessage, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;
    MSG msg;
    HWND hWnd;
    LPTSTR szTitle = TEXT("クライアントアリアが透過するウィンドウ");
    LPTSTR szWindowClass = TEXT("SAMPLE");

    ZeroMemory(&wcx, sizeof(wcx));

    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = static_cast<WNDPROC>(WndProc);
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR0));
    wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcx.lpszMenuName = MAKEINTRESOURCE(WINMENU);
    wcx.lpszClassName = szWindowClass;

    if (!RegisterClassEx(&wcx))  return 0;

    hWnd = CreateWindow(szWindowClass,
                        szTitle,
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        window_width,
                        window_height,
                        HWND_DESKTOP,
                        NULL,
                        hInstance,
                        NULL);

    if (!hWnd)  return 0;

    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT umessage, WPARAM wParam, LPARAM lParam)
{
    LONG64 orgStyle;

    static WORD wTimer = 0;
    static BOOL bTransparent;

    PAINTSTRUCT ps;
    LPCTSTR msgTransparent = TEXT("イベントは透過します");
    LPCTSTR msgNoTransparent = TEXT("イベントは透過しません");
    static TCHAR szMsg[256];

    switch (umessage)
    {
        case WM_SHOWWINDOW:
            wTimer = SetTimer(hWnd, ID_TIMER, 1000, NULL);
            orgStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
            SetWindowLongPtr(hWnd, GWL_EXSTYLE, (orgStyle | WS_EX_LAYERED));
            SetLayeredWindowAttributes(hWnd, 0, 240, LWA_ALPHA);    // 全体の alpha = 240
            ua_tcscpy(szMsg, msgNoTransparent);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case ID_CLOSE:
                    DestroyWindow(hWnd);
                    break;
            
                default:
                    return DefWindowProc(hWnd, umessage, wParam, lParam);
            }
            break;

        case WM_PAINT:
            BeginPaint(hWnd, &ps);
            TextOut(ps.hdc, 10, 10, szMsg, ua_lstrlen(szMsg));
            EndPaint(hWnd, &ps);
            break;

        case WM_TIMER:
            orgStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
            if (bTransparent == TRUE) {
                SetLayeredWindowAttributes(hWnd, 0, 150, LWA_ALPHA);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, (orgStyle | WS_EX_TRANSPARENT));    // イベントも透過
                ua_tcscpy(szMsg, msgTransparent);
                bTransparent = FALSE;
            } else {
                SetLayeredWindowAttributes(hWnd, 0, 240, LWA_ALPHA);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, (orgStyle & ~WS_EX_TRANSPARENT));    // イベント透過をやめる
                ua_tcscpy(szMsg, msgNoTransparent);
                bTransparent = TRUE;
            }
            InvalidateRect(hWnd, NULL, TRUE);   // メッセージ再表示のため無効領域発生
            break;

        case WM_DESTROY:
            if (wTimer) KillTimer(hWnd, ID_TIMER);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, umessage, wParam, lParam);
    }

    return 0;
}
