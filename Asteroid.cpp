#include "Asteroid.h"

Asteroid::Asteroid(POINT position, float velocity, float rotation, int health, Size size) 
	: GameObject(position, velocity, rotation, health), size(size) {
}

Size Asteroid::getSize() {
    return size;
}

RECT Asteroid::getBoundingRect() {
    RECT rect{};
    int numericSizeHalfWidth = static_cast<int>(size) / 2;
    rect.left = position.x - numericSizeHalfWidth;
    rect.top = position.y - numericSizeHalfWidth;
    rect.right = position.x + numericSizeHalfWidth;
    rect.bottom = position.y + numericSizeHalfWidth;
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