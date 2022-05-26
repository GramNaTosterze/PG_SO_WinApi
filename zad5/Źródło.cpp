// GT_HelloWorldWin32.cpp  
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c  

#include <windows.h>  
#include <stdlib.h>  
#include <string.h>  
#include <tchar.h>  
#include <fileapi.h>
#include <strsafe.h>

// Global variables  

// The main window class name.  
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.  
static TCHAR szTitle[] = _T("Windows_Zad5");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);
        return 1;
    }

    hInst = hInstance; // Store instance handle in our global variable  

    // The parameters to CreateWindow explained:  
    // szWindowClass: the name of the application  
    // szTitle: the text that appears in the title bar  
    // WS_OVERLAPPEDWINDOW: the type of window to create  
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
    // 500, 100: initial size (width, length)  
    // NULL: the parent of this window  
    // NULL: this application does not have a menu bar  
    // hInstance: the first parameter from WinMain  
    // NULL: not used in this application  
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:  
    // hWnd: the value returned from CreateWindow  
    // nCmdShow: the fourth parameter from WinMain  
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:  
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)  
//  
//  PURPOSE:  Processes messages for the main window.  
//  
//  WM_PAINT    - Paint the main window  
//  WM_DESTROY  - post a quit message and return  
//  
//  
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR cPath[] = _T("c:\\");

    //numer seryjny
    LPTSTR volumeNameBuffer = NULL, fileSystemNameBuffer = NULL;
    DWORD volumeSerialNumber, maximumComponentLength, fileSystemFlags;
    GetVolumeInformation(cPath, volumeNameBuffer,MAX_PATH+1, &volumeSerialNumber, &maximumComponentLength, &fileSystemFlags, fileSystemNameBuffer,10);
    TCHAR outVolSerNum[40];
    StringCbPrintf(outVolSerNum, 40 * sizeof(TCHAR), TEXT("Numer seryjny woluminu C: %04X-%04X"), HIWORD(volumeSerialNumber), LOWORD(volumeSerialNumber));
    
    //wolne bajty
    DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
    ULARGE_INTEGER FreeBytesToCaller, TotalBytes, FreeBytes;
    GetDiskFreeSpaceEx(cPath, &FreeBytesToCaller, &TotalBytes, &FreeBytes);
    TCHAR outSize[60];
    StringCbPrintf(outSize, 60 * sizeof(TCHAR), TEXT("Liczba bajtów wolnej przestrzeni na C: %llu"), FreeBytes);
    
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc,
            5, 5,
            outVolSerNum, _tcslen(outVolSerNum));
        TextOut(hdc,
            5, 20,
            outSize, _tcslen(outSize));

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}