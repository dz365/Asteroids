// Asteroids.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Asteroids.h"
#include "Player.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Alien.h"
#include "GameObject.h"
#include <unordered_set>
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#include <list>
#include <unordered_map>
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

std::random_device rd; // Create a random number generator engine
std::mt19937 gen(rd()); // Mersenne Twister engine

static int clientWidth, clientHeight;

// Lists containing gameObjects
std::shared_ptr<Player> player = std::make_shared<Player>();
std::list<std::shared_ptr<Asteroid>> asteroidObjects;
std::list<std::shared_ptr<Bullet>> bulletObjects;
std::list<std::shared_ptr<Alien>> alienObjects;
std::list<std::shared_ptr<Bullet>> alienBulletObjects;
std::unordered_map<UINT_PTR, std::shared_ptr<Alien>> alienTimers;

int score = 0;
boolean allowedToFire = true;
int numAliensGenerated = 0;
HWND NEWGAME_BUTTON;
const int BTN_NEWGAME = 1;
std::unordered_set<std::string> pressedKeys;
constexpr UINT_PTR TIMER_ID = 1;
constexpr UINT_PTR GENERATE_ASTEROID_TIMER_ID = 2;
constexpr UINT_PTR GENERATE_ALIEN_TIMER_ID = 3;
constexpr UINT_PTR PLAYER_FIRING_SPEED_TIMER_ID = 4;
constexpr UINT_PTR REFRESH_RATE_TIMER_ID = 5;

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
   SetTimer(hWnd, GENERATE_ASTEROID_TIMER_ID, 2000, nullptr);
   SetTimer(hWnd, GENERATE_ALIEN_TIMER_ID, 10000, nullptr);
   SetTimer(hWnd, PLAYER_FIRING_SPEED_TIMER_ID, 300, nullptr);
   SetTimer(hWnd, REFRESH_RATE_TIMER_ID, 10, nullptr);
   return TRUE;
}

void generateNewAsteroid() {
    std::uniform_int_distribution<int> distribution(0, clientWidth);

    // Generate a random number
    int randomX = distribution(gen);
    int randomY = distribution(gen) % 2 ? 0 : clientHeight;
    int randomRotation = distribution(gen);
    int velocity = 5 + distribution(gen) % 5;
    Size size = static_cast<Size>(distribution(gen) % 3);
    POINT position = { randomX, randomY };
    asteroidObjects.push_back(
        std::make_shared<Asteroid>(position, velocity, randomRotation, size));
}

void generateNewAlien(HWND hWnd) {
    std::uniform_int_distribution<int> distribution(0, clientHeight - 10);
    int randomY = distribution(gen);
    POINT position = { 0, randomY };
    std::shared_ptr<Alien> newAlien = 
        std::make_shared<Alien>(position, 10 + numAliensGenerated);
    alienObjects.push_back(newAlien);
    alienTimers[newAlien->getId()] = newAlien;
    SetTimer(hWnd, newAlien->getId(), 1000, nullptr);
    numAliensGenerated++;
}

void generateAlienBullet(int alienId) {
    // Timer is from an alien and is for shooting bullets;
    if (alienTimers.find(alienId) == alienTimers.end()) return;

    std::shared_ptr<Alien> alien = alienTimers[alienId];
    POINT bulletPosition = alien->getPosition();
    float alienRotation = alien->getRotation();
    alienBulletObjects.push_back(
        std::make_shared<Bullet>(bulletPosition, 15, alienRotation + PI / 2, 1));
}

void renderScore(Gdiplus::Graphics& graphics) {
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 32, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::PointF point(clientWidth / 2 - 16, 10);
    std::wstring scoreStr = std::to_wstring(score);
    graphics.DrawString(scoreStr.c_str(), -1, &font, point, &textBrush);
}

void renderPlayerHealth(Gdiplus::Graphics& graphics) {
    Gdiplus::Image spaceship(L"spaceship.png"); // 24x24px
    for (int i = 0; i < player->getHealth(); i++) {
        graphics.DrawImage(&spaceship, 30 * i + 10, 10);
    }
}

void renderPlayerRemainingBullets(Gdiplus::Graphics& graphics) {
    Gdiplus::Image bullet(L"bullet.png"); // 24x24px
    for (int i = 0; i < player->getBulletsAvailable(); i++) {
        graphics.DrawImage(&bullet, 10 * i + 10, 40);
    }
}

void renderGameObject(Gdiplus::Graphics& graphics, std::shared_ptr<GameObject> obj) {
    Gdiplus::GraphicsState graphicsState = graphics.Save();
    obj->render(graphics);
    // Restore the original state of the Graphics object
    graphics.Restore(graphicsState);
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

    if (allowedToFire &&  pressedKeys.find("SPACE") != pressedKeys.end() && player->getBulletsAvailable() > 0) {
        POINT bulletPosition = player->getPosition();
        float playerRotation = player->getRotation();
        bulletPosition.x += static_cast<LONG>(sin(playerRotation) * 17);
        bulletPosition.y -= static_cast<LONG>(cos(playerRotation) * 17);
        bulletObjects.push_back(std::make_shared<Bullet>(bulletPosition, 15, player->getRotation(), 1));
        player->decreaseBullets();
        allowedToFire = false;
    }
}

bool checkCollision(RECT r1, RECT r2)
{
    return !(
        r1.left > r2.right || r2.left > r1.right ||     // horizontal collision
        r1.bottom < r2.top || r2.bottom < r1.top);      // vertical collision
}

void createNewGame(HWND hWnd) {
    asteroidObjects.clear();
    bulletObjects.clear();

    // Need to stop alien timers first
    for (const auto& pair : alienTimers) {
        KillTimer(hWnd, pair.first);
    }
    alienTimers.clear();
    alienObjects.clear();
    alienBulletObjects.clear();
    pressedKeys.clear();
    score = 0;
    player = std::make_shared<Player>();
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
                createNewGame(hWnd);
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

            for (auto& obj : asteroidObjects) {
                obj->update(hWnd);
            }
            for (auto& obj : bulletObjects) {
                obj->update(hWnd);
            }
            for (auto& obj : alienObjects) {
                obj->update(hWnd);
            }
            for (auto& obj : alienBulletObjects) {
                obj->update(hWnd);
            }

            // Handle player-object collisions
            RECT playerHitbox = player->getBoundingRect();
            for (auto& asteroid : asteroidObjects) {
                if (checkCollision(playerHitbox, asteroid->getBoundingRect())) {
                    player->handleCollision();
                    asteroid->handleCollision();
                    asteroidObjects.remove(asteroid);
                    break;
                }
            }
            for (auto& alien : alienObjects) {
                if (checkCollision(playerHitbox, alien->getBoundingRect())) {
                    player->handleCollision();
                    alien->handleCollision();
                    KillTimer(hWnd, alien->getId());
                    alienTimers.erase(alien->getId());
                    alienObjects.remove(alien);
                    break;
                }
            }

            // Handle bullet-object collisions
            std::list<std::shared_ptr<Bullet>> bulletsToRemove;
            std::list<std::shared_ptr<Asteroid>>asteroidsToRemove;
            std::list<std::shared_ptr<Alien>>aliensToRemove;
            for (auto& bullet : bulletObjects) {
                RECT bulletHitbox = bullet->getBoundingRect();
                // Check asteriod collision
                for (auto& asteroid : asteroidObjects) {
                    if (checkCollision(bulletHitbox, asteroid->getBoundingRect())) {
                        bullet->handleCollision();
                        asteroid->handleCollision();
                        bulletsToRemove.push_back(bullet);
                        asteroidsToRemove.push_back(asteroid);
                        break;
                    }
                }
                // Check alien collision
                for (auto& alien : alienObjects) {
                    if (checkCollision(bulletHitbox, alien->getBoundingRect())) {
                        bullet->handleCollision();
                        alien->handleCollision();
                        bulletsToRemove.push_back(bullet);
                        aliensToRemove.push_back(alien);
                        break;
                    }
                }

            }
            for (auto& bullet : bulletsToRemove) {
                bulletObjects.remove(bullet);
                player->increaseBullets();
            }

            // Handle asteroid fragmentation upon impact
            for (auto& asteroid : asteroidsToRemove) {
                asteroidObjects.remove(asteroid);
                POINT newAsteroidPosition = asteroid->getPosition();
                if (asteroid->getSize() != SMALL) {
                    Size newAsteroidSize = static_cast<Size>(asteroid->getSize() - 1);
                    asteroidObjects.push_back(std::make_shared<Asteroid>(
                        newAsteroidPosition,
                        asteroid->getVelocity(),
                        asteroid->getRotation() - (PI / 2),
                        newAsteroidSize)
                    );
                    asteroidObjects.push_back(std::make_shared<Asteroid>(
                        newAsteroidPosition,
                        asteroid->getVelocity(),
                        asteroid->getRotation() + (PI / 2),
                        newAsteroidSize)
                    );
                }
                score += 50 * (1 + static_cast<int>(asteroid->getSize()));
            }

            for (auto& alien : aliensToRemove) {
                KillTimer(hWnd, alien->getId());
                alienTimers.erase(alien->getId());
                alienObjects.remove(alien);
                score += 200;
            }
            bulletsToRemove.clear();

            // Remove bullets that have travelled past a set distance
            for (auto& bullet : bulletObjects) {
                if (bullet->getDistanceTravelled() > clientWidth) {
                    bulletsToRemove.push_back(bullet);
                }
            }

            for (auto& bullet : bulletsToRemove) {
                bulletObjects.remove(bullet);
                player->increaseBullets();
            }

            // Handle alien bullet-player collision
            bulletsToRemove.clear();
            for (auto& bullet : alienBulletObjects) {
                RECT bulletHitbox = bullet->getBoundingRect();
                // Check player collision
                if (checkCollision(bulletHitbox, playerHitbox)) {
                    bullet->handleCollision();
                    player->handleCollision();
                    bulletsToRemove.push_back(bullet);
                    break;
                }
                else if (bullet->getDistanceTravelled() > clientWidth) {
                    // Check bullet distance traveled
                    bulletsToRemove.push_back(bullet);
                }
            }
            for (auto& bullet : bulletsToRemove) {
                alienBulletObjects.remove(bullet);
            }   
            break;
        }
        case GENERATE_ASTEROID_TIMER_ID:
            generateNewAsteroid();
            break;
        case GENERATE_ALIEN_TIMER_ID:
            if (score < 1000) break;    // Calm before the storm
            generateNewAlien(hWnd);
            break;
        case PLAYER_FIRING_SPEED_TIMER_ID:
            allowedToFire = true;
            break;
        case REFRESH_RATE_TIMER_ID:
            InvalidateRect(hWnd, nullptr, FALSE);
        default:
            generateAlienBullet(wParam);
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
        case VK_SPACE:
            pressedKeys.insert("SPACE");
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
            pressedKeys.erase("SPACE");
            break;
        }
        break;
    case WM_PAINT:
        {
        if (player->getHealth() <= 0) {
            NEWGAME_BUTTON = CreateWindowEx(
                WS_EX_TRANSPARENT,
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
        
            renderPlayerHealth(graphics); 
            renderPlayerRemainingBullets(graphics);
            renderScore(graphics);    

            renderGameObject(graphics, player);

            for (auto& obj : asteroidObjects) {
                renderGameObject(graphics, obj);
            }
            for (auto& obj : bulletObjects) {
                renderGameObject(graphics, obj);
            }
            for (auto& obj : alienObjects) {
                renderGameObject(graphics, obj);
            }
            for (auto& obj : alienBulletObjects) {
                renderGameObject(graphics, obj);
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
