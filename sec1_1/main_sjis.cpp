#undef UNICODE
#include <Windows.h>



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    static int count = 0;
    PAINTSTRUCT ps;
    CHAR testMsg[4096];

    switch(uMessage) {
        case WM_PAINT:
            BeginPaint(hWnd, &ps);
            wsprintf(testMsg, "左クリック = %d", count);
            TextOut(ps.hdc, 10, 20, testMsg, strlen(testMsg));
            EndPaint(hWnd, &ps);
            break;
        
        case WM_LBUTTONDOWN:
            count++;
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
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
    LPCSTR szTitle = "サンプル";
    LPCSTR szWindowClass = "SAMPLE";

    ZeroMemory(&wcx, sizeof(wcx));

    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = 0;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcx.lpszMenuName = NULL;
    wcx.lpszClassName = szWindowClass;

    if (!RegisterClassEx(&wcx)){
        return 0;
    }

    hWnd = CreateWindow(szWindowClass, szTitle,
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        400, 100,
                        HWND_DESKTOP, NULL, hInstance, NULL);
    
    if (!hWnd) {
        return 0;
    }

    while(GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
