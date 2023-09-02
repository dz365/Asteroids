#include "StartMenuPanel.h"
#include <string>
#include "PlayingPanel.h"


StartMenuPanel::StartMenuPanel(HWND hwnd) : 
    Panel(hwnd), NEWGAME_BUTTON(CreateWindowEx(
        WS_EX_TRANSPARENT,
        L"BUTTON",
        L"New Game",
        WS_CHILD,
        0, 0, 100, 30,
        hwnd,
        (HMENU)NEWGAME_BUTTON_ID,
        GetModuleHandle(NULL),
        NULL))
{
    SetTimer(hwnd, RENDER_ID, 10, nullptr);
}

void StartMenuPanel::render(Gdiplus::Graphics& graphics)
{
    graphics.Clear(Gdiplus::Color::Black);

    RECT rect;
    GetClientRect(hwnd, &rect);
    int screenWidth = rect.right - rect.left;
    
    const wchar_t* text = L"Asteroids";

    // Define the font and size
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 64, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    // Measure the size of the text
    Gdiplus::RectF textBounds;
    graphics.MeasureString(text, -1, &font, Gdiplus::PointF(0, 0), &textBounds);

    // Calculate text position
    float centerX = (screenWidth - textBounds.Width) / 2;
    float centerY = 25.0f;

    // Create a brush for the text color
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));

    graphics.DrawString(text, -1, &font, Gdiplus::PointF(centerX, centerY), &textBrush);


    SetWindowPos(NEWGAME_BUTTON, NULL, screenWidth / 2 - 50, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    ShowWindow(NEWGAME_BUTTON, SW_SHOW);
}

void StartMenuPanel::handleKeyAction(KeyAction action, WPARAM key)
{
}

void StartMenuPanel::handleTimerAction(UINT_PTR timerId)
{
    KillTimer(hwnd, RENDER_ID);
    InvalidateRect(hwnd, nullptr, TRUE);
}

void StartMenuPanel::handleButtonClickedAction(UINT_PTR buttonId)
{
    SetFocus(hwnd);
    ShowWindow(NEWGAME_BUTTON, SW_HIDE);

    std::shared_ptr<PlayingPanel> panel = std::make_shared<PlayingPanel>(hwnd);
    panel->setGamePanelContext(context);
    context->changePanel(panel);
}
