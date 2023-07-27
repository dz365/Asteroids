#include "Asteroid.h"

Asteroid::Asteroid(POINT position, float velocity, float rotation) 
	: GameObject(position, velocity, rotation) {}

RECT Asteroid::getBoundingRect() {
    RECT rect{};
    rect.left = position.x;
    rect.top = position.y;
    rect.right = position.x + 64;
    rect.bottom = position.y + 64;
    return rect;
}

void Asteroid::update(const HWND& hWnd) 
{
    // Update position
    position.x += static_cast<LONG>(sin(rotation) * velocity);
    position.y -= static_cast<LONG>(cos(rotation) * velocity);

    RECT rect;
    GetClientRect(hWnd, &rect);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    if (position.x < 0)
        position.x = windowWidth;
    if (position.x > windowWidth)
        position.x = 0;
    if (position.y < 0)
        position.y = windowHeight;
    if (position.y > windowHeight)
        position.y = 0;
};
void Asteroid::render(Gdiplus::Graphics& graphics)
{
	Gdiplus::Image asteroid(L"asteroid.png"); // 58x61px
	graphics.DrawImage(&asteroid, static_cast<int>(position.x), static_cast<int>(position.y));
};