#include "Asteroid.h"

Asteroid::Asteroid(POINT position, float velocity, float rotation, int health) 
	: GameObject(position, velocity, rotation, health) {}

RECT Asteroid::getBoundingRect() {
    RECT rect{};
    rect.left = position.x - 30;
    rect.top = position.y - 30;
    rect.right = position.x + 30;
    rect.bottom = position.y + 30;
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