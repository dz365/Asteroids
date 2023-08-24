#include "Asteroid.h"

Asteroid::Asteroid(POINT position, float velocity, float rotation, int health, Size size) 
	: GameObject(position, velocity, rotation, health), size(size) {
}

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

    int numericSize = static_cast<int>(size);
    graphics.DrawImage(
        &asteroid,
        static_cast<int>(position.x - numericSize / 2),
        static_cast<int>(position.y - numericSize / 2),
        numericSize,
        numericSize);
};