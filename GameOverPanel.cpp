#include "GameOverPanel.h"
#include <string>
#include "PlayingPanel.h"

const int BTN_NEWGAME = 1;
HWND NEWGAME_BUTTON = nullptr;


GameOverPanel::GameOverPanel(
    HWND hwnd, std::list<std::shared_ptr<GameObject>> gameObjects, int score) : 
    Panel(hwnd), score(score), gameObjects(gameObjects)
{
    NEWGAME_BUTTON = CreateWindowEx(
        WS_EX_TRANSPARENT,
        L"BUTTON",
        L"New Game",
        WS_CHILD,
        0, 0, 100, 30,
        hwnd,
        (HMENU)BTN_NEWGAME,  // Set a unique ID for the button
        GetModuleHandle(NULL),
        NULL
    );
}

void GameOverPanel::render(Gdiplus::Graphics& graphics)
{
    graphics.Clear(Gdiplus::Color::Black);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int screenWidth = clientRect.right - clientRect.left;
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 32, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::PointF point(screenWidth / 2 - 16, 50);
    std::wstring scoreStr = std::to_wstring(score);
    graphics.DrawString(scoreStr.c_str(), -1, &font, point, &textBrush);

    SetWindowPos(NEWGAME_BUTTON, NULL, screenWidth / 2 - 50, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    ShowWindow(NEWGAME_BUTTON, SW_SHOW);

    for (auto& obj : gameObjects) {
        Gdiplus::GraphicsState graphicsState = graphics.Save();
        obj->render(graphics);
        // Restore the original state of the Graphics object
        graphics.Restore(graphicsState);
    }
    
}

void GameOverPanel::handleKeyAction(KeyAction action, WPARAM key)
{
}

void GameOverPanel::handleTimerAction(UINT_PTR timerId)
{
}

void GameOverPanel::handleButtonClickedAction(UINT_PTR buttonId)
{
    SetFocus(hwnd);
    ShowWindow(NEWGAME_BUTTON, SW_HIDE);

    std::shared_ptr<PlayingPanel> panel = std::make_shared<PlayingPanel>(hwnd);
    panel->setGamePanelContext(context);
    context->changePanel(panel);
}
