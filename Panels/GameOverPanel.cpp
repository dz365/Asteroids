#include "GameOverPanel.h"
#include <string>
#include "PlayingPanel.h"
#include "StartMenuPanel.h"

GameOverPanel::GameOverPanel(
    HWND hwnd, std::list<std::shared_ptr<GameObject>> gameObjects, int score) : 
    Panel(hwnd), score(score), gameObjects(gameObjects),
    NEWGAME_BUTTON(CreateWindowEx(
        WS_EX_TRANSPARENT,
        L"BUTTON",
        L"New Game",
        WS_CHILD,
        0, 0, 100, 30,
        hwnd,
        (HMENU)NEWGAME_BUTTON_ID,
        GetModuleHandle(NULL),
        NULL)),
    STARTMENU_BUTTON(CreateWindowEx(
        WS_EX_TRANSPARENT,
        L"BUTTON",
        L"Start Menu",
        WS_CHILD,
        0, 0, 100, 30,
        hwnd,
        (HMENU)STARTMENU_BUTTON_ID,
        GetModuleHandle(NULL),
        NULL))
{
}

void GameOverPanel::render(Gdiplus::Graphics& graphics)
{
    graphics.Clear(Gdiplus::Color::Black);

    RECT rect;
    GetClientRect(hwnd, &rect);
    int screenWidth = rect.right - rect.left;

    std::wstring text = std::to_wstring(score);;

    // Define the font and size
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 64, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    // Measure the size of the text
    Gdiplus::RectF textBounds;
    graphics.MeasureString(text.c_str(), -1, &font, Gdiplus::PointF(0, 0), &textBounds);

    // Calculate text position
    float centerX = (screenWidth - textBounds.Width) / 2;
    float centerY = 25.0f;

    // Create a brush for the text color
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));

    graphics.DrawString(text.c_str(), -1, &font, Gdiplus::PointF(centerX, centerY), &textBrush);

    SetWindowPos(NEWGAME_BUTTON, NULL, screenWidth / 2 - 50, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    ShowWindow(NEWGAME_BUTTON, SW_SHOW);

    SetWindowPos(STARTMENU_BUTTON, NULL, screenWidth / 2 - 50, 200, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    ShowWindow(STARTMENU_BUTTON, SW_SHOW);

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
    ShowWindow(STARTMENU_BUTTON, SW_HIDE);
    std::shared_ptr<Panel> panel;
    if (buttonId == NEWGAME_BUTTON_ID) {
        panel = std::make_shared<PlayingPanel>(hwnd);
    } else {
        panel = std::make_shared<StartMenuPanel>(hwnd);
    }
    panel->setGamePanelContext(context);
    context->changePanel(panel);
}
