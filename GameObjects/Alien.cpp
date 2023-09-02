#include "Alien.h"

constexpr auto PI = 3.14159265358979323846;
const int ALIEN_VELOCITY = 5;

Alien::Alien(POINT position, int id)
	: GameObject(position, ALIEN_VELOCITY, PI / 2, 1), id(id) {}

int Alien::getId()
{
    return id;
}

RECT Alien::getBoundingRect()
{
    RECT rect{};
    rect.left = position.x - 14;
    rect.top = position.y - 9;
    rect.right = position.x + 14;
    rect.bottom = position.y + 9;
    return rect;
}

void Alien::render(Gdiplus::Graphics& graphics)
{
    Gdiplus::Image alien(L"Resources\\alien.png"); // 28x17px

    graphics.TranslateTransform(position.x, position.y);
    graphics.RotateTransform(rotation * 180 / 3.14159265358979323846);
    graphics.TranslateTransform(-position.x, -position.y);

    graphics.DrawImage(
        &alien,
        static_cast<int>(position.x - 14),
        static_cast<int>(position.y - 9)
    );
}

