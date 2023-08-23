// Asteroids.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Asteroids.h"
#include "Player.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "GameObject.h"
#include <unordered_set>
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#include <list>
#include <cmath>
#include <random>

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

// Lists containing gameObjects
Player* player = new Player();
std::list<Asteroid*> asteroidObjects;
std::list<Bullet*> bulletObjects;

int score = 0;
HWND NEWGAME_BUTTON;
const int BTN_NEWGAME = 1;
std::unordered_set<std::string> pressedKeys;
constexpr UINT_PTR TIMER_ID = 1;
constexpr UINT_PTR GENERATE_ASTEROID_TIMER_ID = 2;

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

   SetTimer(hWnd, TIMER_ID, 50, nullptr);
   SetTimer(hWnd, GENERATE_ASTEROID_TIMER_ID, 3000, nullptr);

   return TRUE;
}

void pressedKeysHandler()
{
    if (pressedKeys.find("W") == pressedKeys.end())
    {
        player->stopEngine();
    } 
    else
    {
        player->startEngine();
    }

    if (pressedKeys.find("A") != pressedKeys.end())
        player->setRotation(player->getRotation() - PI / 12);

    if (pressedKeys.find("D") != pressedKeys.end())
        player->setRotation(player->getRotation() + PI / 12);

    if (pressedKeys.find("SPACE") != pressedKeys.end() && player->getBulletsAvailable() > 0) {
        POINT bulletPosition = player->getPosition();
        float playerRotation = player->getRotation();
        bulletPosition.x += static_cast<LONG>(sin(playerRotation) * 17);
        bulletPosition.y -= static_cast<LONG>(cos(playerRotation) * 17);
        Bullet* bullet = new Bullet(bulletPosition, 15, player->getRotation(), 1);
        bulletObjects.push_back(bullet);
        player->decreaseBullets();
        pressedKeys.erase("SPACE");
    }
}

bool checkCollision(RECT r1, RECT r2)
{
    return !(
        r1.left > r2.right || r2.left > r1.right ||     // horizontal collision
        r1.bottom < r2.top || r2.bottom < r1.top);      // vertical collision
}

void createNewGame() {
    asteroidObjects.clear();
    bulletObjects.clear();
    score = 0;
    player = new Player();
}
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
    static int clientWidth, clientHeight;

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
            clientWidth = LOWORD(lParam);
            clientHeight = HIWORD(lParam);
            hBitmapBuffer = CreateCompatibleBitmap(GetDC(hWnd), clientWidth, clientHeight);
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
                createNewGame();
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
        switch (wParam) {
        case TIMER_ID: {
            if (player->getHealth() <= 0)
                break;
            pressedKeysHandler();
            player->update(hWnd);

            for (GameObject* obj : asteroidObjects) {
                obj->update(hWnd);
            }
            for (GameObject* obj : bulletObjects) {
                obj->update(hWnd);
            }

            // Handle player-object collisions
            RECT playerHitbox = player->getBoundingRect();
            for (Asteroid* asteroid : asteroidObjects) {
                if (checkCollision(playerHitbox, asteroid->getBoundingRect())) {
                    player->handleCollision();
                    asteroid->handleCollision();
                    asteroidObjects.remove(asteroid);
                    break;
                }
            }

            // Handle bullet-object collisions
            std::list<Bullet*> bulletsToRemove;
            for (Bullet* bullet : bulletObjects) {
                RECT bulletHitbox = bullet->getBoundingRect();
                for (Asteroid* asteroid : asteroidObjects) {
                    if (checkCollision(bulletHitbox, asteroid->getBoundingRect())) {
                        bullet->handleCollision();
                        asteroid->handleCollision();
                        asteroidObjects.remove(asteroid);
                        delete asteroid;
                        bulletsToRemove.push_back(bullet);
                        score += 100;
                        break;
                    }
                }
            }
            for (Bullet* bullet : bulletsToRemove) {
                bulletObjects.remove(bullet);
                delete bullet;
                player->increaseBullets();
            }

            bulletsToRemove.clear();

            // Remove bullets that have travelled past a set distance
            for (Bullet* bullet : bulletObjects) {
                if (bullet->getDistanceTravelled() > clientWidth) {
                    bulletsToRemove.push_back(bullet);
                }
            }

            for (Bullet* bullet : bulletsToRemove) {
                bulletObjects.remove(bullet);
                delete bullet;
                player->increaseBullets();
            }

            InvalidateRect(hWnd, nullptr, FALSE);
            break;
        }
        case GENERATE_ASTEROID_TIMER_ID:
            // Create a random number generator engine
            std::random_device rd;
            std::mt19937 gen(rd()); // Mersenne Twister engine
            std::uniform_int_distribution<int> distribution(0, clientWidth);

            // Generate a random number
            int randomX = distribution(gen) % clientWidth;
            int randomY = distribution(gen) % 2 ? 0 : clientHeight;
            int randomRotation = distribution(gen);
            asteroidObjects.push_back(new Asteroid({ randomX, randomY }, 5, randomRotation, 1));
            break;
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case 'W':
            pressedKeys.insert("W");
            break;
        case 'A':
            pressedKeys.insert("A");
            break;
        case 'D':
            pressedKeys.insert("D");
            break;
        }
        break;
    case WM_KEYUP:
        switch (wParam) {
        case 'W':
            pressedKeys.erase("W");
            break;
        case 'A':
            pressedKeys.erase("A");
            break;
        case 'D':
            pressedKeys.erase("D");
            break;
        case VK_SPACE:
            pressedKeys.insert("SPACE");
            break;
        }
        break;
    case WM_PAINT:
        {
        if (player->getHealth() <= 0) {
            NEWGAME_BUTTON = CreateWindowEx(
                0,
                L"BUTTON",
                L"New Game",
                WS_VISIBLE | WS_CHILD,
                10, 10, 100, 30,
                hWnd,
                (HMENU)BTN_NEWGAME,  // Set a unique ID for the button
                GetModuleHandle(NULL),
                NULL
            );
        }
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            Gdiplus::Graphics graphics(hdcBuffer);
            graphics.Clear(Gdiplus::Color::Black);
            // Save the current state of the Graphics object
            
            Gdiplus::Image spaceship(L"spaceship.png"); // 24x24px

            for (int i = 0; i < player->getHealth(); i++) {
                graphics.DrawImage(&spaceship, 30 * i + 10, 10);
            }
            
            Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));
            Gdiplus::FontFamily fontFamily(L"Arial");
            Gdiplus::Font font(&fontFamily, 32, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
            Gdiplus::PointF point(clientWidth / 2 - 16, 10);
            std::wstring scoreStr = std::to_wstring(score);
            graphics.DrawString(scoreStr.c_str(), -1, &font, point, &textBrush);

            Gdiplus::GraphicsState graphicsState = graphics.Save();
            player->render(graphics);
            // Restore the original state of the Graphics object
            graphics.Restore(graphicsState);

            for (GameObject* obj : asteroidObjects) {
                Gdiplus::GraphicsState graphicsState = graphics.Save();
                obj->render(graphics);
                // Restore the original state of the Graphics object
                graphics.Restore(graphicsState);
            }

            for (GameObject* obj : bulletObjects) {
                Gdiplus::GraphicsState graphicsState = graphics.Save();
                obj->render(graphics);
                // Restore the original state of the Graphics object
                graphics.Restore(graphicsState);
            }

            // Copy the entire buffered content to the window DC
            BitBlt(hdc, 0, 0, clientWidth, clientHeight, hdcBuffer, 0, 0, SRCCOPY);

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
