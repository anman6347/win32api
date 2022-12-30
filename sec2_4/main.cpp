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
    LPTSTR szTitle = TEXT("透過するウィンドウ");
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
    static WORD wTimer = 0;
    static BYTE bAlpha = 255;
    LONG64 orgStyle;

    switch (umessage)
    {
        case WM_SHOWWINDOW:
            orgStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
            SetWindowLongPtr(hWnd, GWL_EXSTYLE, (orgStyle | WS_EX_LAYERED));
            SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);    // alpha = 255
            wTimer = SetTimer(hWnd, ID_TIMER, 1, NULL);             // 1ms タイマーの生成
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

        case WM_TIMER:
            SetLayeredWindowAttributes(hWnd, 0, bAlpha, LWA_ALPHA);     // alpha = bAlpha
            bAlpha--;       // BYTE なので 0 の次は 255
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
