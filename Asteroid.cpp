#include "Asteroid.h"

Asteroid::Asteroid(POINT position, float velocity, float rotation, Size size) 
	: GameObject(position, velocity, rotation, 1), size(size) {
}

Size Asteroid::getSize() {
    return size;
}

RECT Asteroid::getBoundingRect() {
    RECT rect{};
    int numericSizeHalfWidth = 15 * (static_cast<int>(size) + 2) / 2;
    rect.left = position.x - numericSizeHalfWidth;
    rect.top = position.y - numericSizeHalfWidth;
    rect.right = position.x + numericSizeHalfWidth;
    rect.bottom = position.y + numericSizeHalfWidth;
    return rect;
}

void Asteroid::render(Gdiplus::Graphics& graphics)
{
	Gdiplus::Image asteroid(L"asteroid.png"); // 58x61px

    int numericSize = 15 * (static_cast<int>(size) + 2);
    graphics.DrawImage(
        &asteroid,
        static_cast<int>(position.x - numericSize / 2),
        static_cast<int>(position.y - numericSize / 2),
        numericSize,
        numericSize);
};