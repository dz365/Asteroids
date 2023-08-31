// Asteroids.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Asteroids.h"
#include <objidl.h>
#include <gdiplus.h>
#include "GamePanelContext.h"
#include "PlayingPanel.h"
#include <memory>
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100
constexpr auto PI = 3.14159265358979323846;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



HWND NEWGAME_BUTTON;
const int BTN_NEWGAME = 1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASTEROIDS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTEROIDS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTEROIDS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASTEROIDS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//void createNewGame(HWND hWnd) {
//    // Reset game objects
//    asteroidObjects.clear();
//    bulletObjects.clear();
//
//    // Stop all alien firing timers.
//    for (const auto& pair : alienTimers) {
//        KillTimer(hWnd, pair.first);
//    }
//    alienTimers.clear();
//    alienObjects.clear();
//    alienBulletObjects.clear();
//
//    
//    pressedKeys.clear();
//    score = 0;
//
//    
//
//    panel = PLAYING;
//
//}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmapBuffer = NULL;
    static HDC hdcBuffer = NULL;

    static std::shared_ptr<PlayingPanel> panel = std::make_shared<PlayingPanel>(hWnd);
    static std::shared_ptr<GamePanelContext> panelContext = std::make_shared<GamePanelContext>(panel);

    switch (message)
    {
    case WM_CREATE:
        // Create the memory DC for double buffering
        hdcBuffer = CreateCompatibleDC(NULL);
        break;

    case WM_SIZE:
        // Recreate the buffer when the window size changes
        if (hdcBuffer) {
            if (hBitmapBuffer) DeleteObject(hBitmapBuffer);
            hBitmapBuffer = CreateCompatibleBitmap(
                GetDC(hWnd), LOWORD(lParam), HIWORD(lParam));
            SelectObject(hdcBuffer, hBitmapBuffer);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case BTN_NEWGAME:
                SetFocus(hWnd);
                ShowWindow(NEWGAME_BUTTON, SW_HIDE);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_TIMER:
        panelContext->handleTimerAction(wParam);
        break;
    case WM_KEYDOWN:
        panelContext->handleKeyAction(KeyAction::KEYDOWN, wParam);
        break;
    case WM_KEYUP:
        panelContext->handleKeyAction(KeyAction::KEYUP, wParam);
        break;
    case WM_PAINT:
        {
        //if (player->getHealth() <= 0) {
        //    NEWGAME_BUTTON = CreateWindowEx(
        //        WS_EX_TRANSPARENT,
        //        L"BUTTON",
        //        L"New Game",
        //        WS_VISIBLE | WS_CHILD,
        //        10, 10, 100, 30,
        //        hWnd,
        //        (HMENU)BTN_NEWGAME,  // Set a unique ID for the button
        //        GetModuleHandle(NULL),
        //        NULL
        //    );
        //}
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int screenWidth = clientRect.right - clientRect.left;
        int screenHeight = clientRect.bottom - clientRect.top;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Gdiplus::Graphics graphics(hdcBuffer);

        panelContext->render(graphics);
        // Copy the entire buffered content to the window DC
        BitBlt(hdc, 0, 0, screenWidth, screenHeight, hdcBuffer, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        if (hdcBuffer) {
            DeleteDC(hdcBuffer);
            hdcBuffer = NULL;
        }
        if (hBitmapBuffer) {
            DeleteObject(hBitmapBuffer);
            hBitmapBuffer = NULL;
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
