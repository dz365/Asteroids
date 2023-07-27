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

void Asteroid::render(Gdiplus::Graphics& graphics)
{
	Gdiplus::Image asteroid(L"asteroid.png"); // 58x61px
	graphics.DrawImage(
        &asteroid, 
        static_cast<int>(position.x - 26), 
        static_cast<int>(position.y - 30));
};