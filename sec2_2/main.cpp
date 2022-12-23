#include <stdio.h>
#include <conio.h>      // kbhit(), getch()
#include <windows.h>
#include "resource.h"


PBITMAPINFOHEADER pDib;     // 情報ヘッダ + データ本体 のバッファの先頭ポインタ
LPBYTE pBitmap;             // データ本体のバッファの先頭ポインタ


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
int getFname(char *inFname);
void extractFname(char *inFname, char *fNameWoPath);
BOOL readBmpHeader(FILE *fp, PBITMAPFILEHEADER bmFileHdr);
BOOL readBmpBody(FILE *fp, int bitmapSize, PBITMAPINFOHEADER pDib);
HWND crtWindow(PBITMAPINFOHEADER pDib);

int main (int argc, char *argv[])
{
    FILE *fp;
    HWND hWnd;
    MSG msg;
    int bitmapSize;
    BITMAPFILEHEADER bmFileHdr;
    char fName[_MAX_PATH], dfFile[_MAX_EXT];

    if (argc < 2) {     // 引数無しの場合はダイアログの表示でファイル名を取得 
        if (getFname(fName) < 0) return 1;
    } else {
        strcpy(fName, argv[1]); // 引数有りの場合
    }

    extractFname(fName, dfFile);
    fprintf(stdout, "ファイル名 = %s\n", dfFile);


    if ((fp = fopen(fName, "rb")) == NULL) {
        fprintf(stderr, "エラー : ファイルのオープンに失敗\n");
        return 1;
    }

    if (!readBmpHeader(fp, &bmFileHdr)) {
        fprintf(stderr, "エラー : ファイルの読み込み失敗\n");
        fclose(fp);
        return 1;
    }

    // 情報ヘッダを含めたビットマップデータ用のメモリ確保
    bitmapSize = bmFileHdr.bfSize - sizeof(BITMAPFILEHEADER);
    pDib = static_cast<BITMAPINFOHEADER*>(malloc(bitmapSize));  // 後で先頭部分の情報ヘッダを読み取るため, pDib で読み取れるように型を変換

    // データ本体読み取り
    if (!readBmpBody(fp, bitmapSize, pDib)) {
        fprintf(stderr, "エラー : ファイル読み込み失敗\n");
        free(pDib);
        fclose(fp);
        return 1;
    }
    fclose(fp);

    // ポインタをデータ先頭に移動
    pBitmap = reinterpret_cast<BYTE*>(pDib) + (bmFileHdr.bfOffBits - sizeof(BITMAPFILEHEADER));

    // ウィンドウ表示
    hWnd = crtWindow(pDib);

    if (hWnd == NULL)
        return 1;

    fprintf(stdout, "Enter で終了...");

    // メッセージループ
    msg.message = WM_CREATE;
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (kbhit()) {  // Enter が押された場合
            PostMessage(hWnd, (UINT)WM_DESTROY, (UINT)0, (LONG)0);
            getch();
        }
    }
    free(pDib);

    return 0;
}


/**
 * @brief ファイル名の取得
 * @param inFname ファイル名文字列へのポインタ
 * @return int 成功時 0, 失敗時 -1
 * @details 「ファイルを開く」ダイアログを生成する
 */
int getFname(char *inFname)
{
    OPENFILENAME fName;
    WCHAR fileName[256];
    fileName[0] = L'\0';
    LPCWSTR filefileter = L"画像ファイル(*.bmp)\0*.bmp\0\0";

    memset(&fName, 0, sizeof(OPENFILENAME));
    fName.lStructSize = sizeof(OPENFILENAME);               // 構造体のサイズ
    fName.lpstrFilter = filefileter;                        // *.bmp のみ読み取ることにする
    fName.nFilterIndex = 1;                                 // 現在選択しているフィルターのインデックス (1-origin)
    fName.lpstrFile = fileName;                             // ドライブ指定子, パス, ファイル名, 拡張子が入るバッファ
    fName.nMaxFile = sizeof(fileName);                      // lpstrFile バッファのサイズ
    fName.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;     // ダイアログボックスのフラグ

    if (GetOpenFileName(&fName) == FALSE) {
        return -1;
    }

    char tmp[256];
    wcstombs(tmp, fileName, sizeof(tmp));
    strcpy(inFname, tmp);

    return 0;
}

/**
 * @brief パスが付いたファイル名からパスを取り除く
 * @param inFname パス付きファイル名文字列へのポインタ
 * @param fNameWoPath パスを取り除いたファイル名を格納するバッファへのポインタ
 */
void extractFname(char *inFname, char *fNameWoPath)
{
    char dfName[_MAX_FNAME], dfExt[_MAX_EXT];

    _splitpath(inFname, NULL, NULL, dfName, dfExt);
    strcpy(fNameWoPath, dfName);
    strcat(fNameWoPath, dfExt);

    return;
}

/**
 * @brief ヘッダーを読み取って, ビットマップファイルかチェックする
 * @param fp ファイルポインタ
 * @param bmFileHdr ヘッダー読み取り用のバッファ
 * @return BOOL 成功時 TRUE, 失敗時 FALSE
 */
BOOL readBmpHeader(FILE *fp, PBITMAPFILEHEADER bmFileHdr)
{
    // BITMAPFILEHEADER のサイズ分読み込む
    if (fread(bmFileHdr, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
        return FALSE;
    }

    // bmp かチェック
    if (bmFileHdr->bfType != 'M' * 256 + 'B')
        return FALSE;

    return TRUE;
}

/**
 * @brief 24 bpp ビットマップファイルの情報ヘッダからデータ本体を読み取る. readBmpHeader 関数を先に呼び出している必要がある.
 * @param fp ファイルポインタ
 * @param bitmapSize 情報ヘッダとデータ本体のサイズ
 * @param pDib 情報ヘッダから先を格納しているバッファ
 * @return 成功時 TRUE, 失敗時 FALSE
 */
BOOL readBmpBody(FILE *fp, int bitmapSize, PBITMAPINFOHEADER pDib)
{
    if (fread(pDib, bitmapSize, 1, fp) != 1)    // readBmpHeader を呼び出した後なら, 読み込み開始位置が情報ヘッダ先頭になっている
        return FALSE;

    fprintf(stdout, "biBitCount = %ld\n", pDib->biClrUsed);

    return TRUE;
}

/**
 * @brief bmp ファイルの大きさに合わせたウィンドウを生成して, ウィンドウハンドルを取得する
 * @param pDib bmp 情報ヘッダ先頭のポインタ
 * @return HWND 成功時 取得したウィンドウハンドル, 失敗時 NULL
 */
HWND crtWindow(PBITMAPINFOHEADER pDib)
{
    //HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(NULL, GWLP_HINSTANCE);    // 現在のインスタンスを取得
    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
    HWND hWnd = NULL;

    LPTSTR szTitle = TEXT("ビットマップ表示");
    LPTSTR szWindowClass = TEXT("SAMPLE");
    WNDCLASSEX wcx;
    ZeroMemory(&wcx, sizeof(wcx));

    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.lpfnWndProc = WndProc;
    wcx.hInstance = hInstance;
    wcx.lpszClassName = szWindowClass;
    wcx.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR0));
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON0));
    wcx.lpszMenuName = MAKEINTRESOURCE(WINMENU);

    if (!RegisterClassEx(&wcx))
        return NULL;


    // ウィンドウ生成

    int windowWidth = pDib->biWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
    int windowHeight = pDib->biHeight + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFIXEDFRAME) * 2;



    hWnd = CreateWindow(szWindowClass,
                        szTitle,
                        WS_OVERLAPPED| WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        windowWidth,
                        windowHeight,
                        HWND_DESKTOP,
                        NULL,
                        hInstance,
                        NULL);

    return hWnd;
}

/**
 * @brief ウィンドウプロシージャ
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;

    switch (uMessage) {
        case WM_PAINT:
            BeginPaint(hWnd, &ps);
            SetDIBitsToDevice(ps.hdc,
                              0,
                              0,
                              pDib->biWidth,
                              pDib->biHeight,
                              0,
                              0,
                              0,
                              pDib->biHeight,
                              pBitmap,
                              reinterpret_cast<BITMAPINFO*>(pDib),
                              DIB_RGB_COLORS);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

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
            break;
    }
    return 0;
}
