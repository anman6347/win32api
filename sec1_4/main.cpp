#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    LPCWSTR testMsg = L"左クリックで青、右クリックで赤の四角形を描画します";
    HDC hdc;
    RECT rc;
    HANDLE hOldPen, hPen;

    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_DRAW:
            PostMessage(hWnd, WM_RBUTTONUP, 0, 0);
            PostMessage(hWnd, WM_LBUTTONUP, 0, 0);
            break;

        case ID_ERASE:
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        
        case ID_CLOSE:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        TextOut(ps.hdc, 10, 20, testMsg, wcslen(testMsg));
        EndPaint(hWnd, &ps);
        break;

    case WM_RBUTTONUP:
        hdc = GetDC(hWnd);
        GetClientRect(hWnd, &rc);  // ウィンドウサイズ取得

        hPen = CreatePen(PS_SOLID, 1, (COLORREF)(RGB(255, 0, 0)));
        hOldPen = SelectObject(hdc, hPen);  // 作成したペンを選択して保持

        Rectangle(hdc, rc.left + 10, rc.top + 10,
                        (rc.right / 2) - 10, (rc.bottom / 2) - 10);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        ReleaseDC(hWnd, hdc);
        break;

    case WM_LBUTTONUP:
        hdc = GetDC(hWnd);
        GetClientRect(hWnd, &rc);

        hPen = CreatePen(PS_SOLID, 1, (COLORREF)(RGB(0, 0, 255)));
        hOldPen = SelectObject(hdc, hPen);

        Rectangle(hdc, (rc.right / 2) + 10, (rc.bottom / 2) + 10,
                        rc.right - 10, rc.bottom - 10);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        ReleaseDC(hWnd, hdc);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;
    MSG msg;
    HWND hWnd;
    LPCWSTR szTitle = L"メッセージ処理";
    LPCWSTR szWindowClass = L"SAMPLE";

    ZeroMemory(&wcx, sizeof(wcx));
    wcx.cbSize          = sizeof(WNDCLASSEX);
    wcx.style           = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc     = WndProc;
    wcx.cbClsExtra      = 0;
    wcx.cbWndExtra      = 0;
    wcx.hInstance       = hInstance;
    wcx.hIcon           = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.hIconSm         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.hCursor         = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground   = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcx.lpszMenuName    = MAKEINTRESOURCE(WINMENU);
    wcx.lpszClassName   = szWindowClass;

    if (!RegisterClassEx(&wcx)) {
        return 0;
    }

    hWnd = CreateWindow(szWindowClass, szTitle,
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        GetSystemMetrics(SM_CXSCREEN) / 2 - 500 / 2,
                        GetSystemMetrics(SM_CYSCREEN) / 2 - 200 / 2,
                        500, 200,
                        HWND_DESKTOP, NULL, hInstance, NULL);

    if (!hWnd) {
        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
