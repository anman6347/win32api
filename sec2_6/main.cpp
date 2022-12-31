#include <windows.h>
#include "resource.h"


const int window_width = 500;
const int window_height = 500;

LRESULT CALLBACK WndProc(HWND hWnd, UINT umessage, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;
    MSG msg;
    HWND hWnd;
    LPTSTR szTitle = TEXT("変形ウィンドウ");
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
    static HRGN hRgn = NULL;
    const POINT p[9] = {
        {0, 0},
        {249, 124},
        {499, 0},
        {249, 499}
    };

    PAINTSTRUCT ps;
    LPCTSTR szMsg1 = TEXT("マウスの左ボタンでウィンドウが変形します.");
    LPCTSTR szMsg2 = TEXT("マウスの右ボタンでウィンドウの変形を戻します.");


    switch (umessage)
    {
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case ID_CLOSE:
                DestroyWindow(hWnd);
                break;
            
            default:
                DefWindowProc(hWnd, umessage, wParam, lParam);
                break;
            }

        case WM_PAINT:
            BeginPaint(hWnd, &ps);
            TextOut(ps.hdc, 80, 100, szMsg1, ua_lstrlen(szMsg1));
            TextOut(ps.hdc, 80, 120, szMsg2, ua_lstrlen(szMsg2));
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDOWN:
            hRgn = CreatePolygonRgn(p, 4, WINDING);
            SetWindowRgn(hWnd, hRgn, TRUE);
            break;

        case WM_RBUTTONDOWN:
            SetWindowRgn(hWnd, NULL, TRUE);
            break;

        default:
            return DefWindowProc(hWnd, umessage, wParam, lParam);
    }

    return 0;
}
