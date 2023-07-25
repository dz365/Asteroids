#include "Player.h"
#include <cmath>

Player::Player() : GameObject({ 150, 150 }, 0, 0) 
{
    engineOn = false;
}

void Player::startEngine() {
	engineOn = true;
}

void Player::stopEngine() {
    engineOn = false;
}

RECT Player::getBoundingRect() {
    RECT rect{};
    rect.left = position.x - 10;
    rect.top = position.y;
    rect.right = position.x + 10;
    rect.bottom = position.y + 40;
    return rect;
}

void Player::update(const HWND& hWnd)
{
    // Determine new velocity
    if (engineOn)
        velocity += 2.0f;
    else
        velocity -= 0.5f;
    if (velocity < 0)
        velocity = 0;
    if (velocity > 12)
        velocity = 12.0f;

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
}

void Player::render(const HDC& hdc) 
{
	// Draw spaceship
    Gdiplus::Image spaceship(L"spaceship.png");

    Gdiplus::Graphics graphics(hdc);

    graphics.TranslateTransform(position.x + 12, position.y + 12);
    graphics.RotateTransform(rotation * 180 / 3.14159265358979323846);
    graphics.TranslateTransform(-position.x - 12, -position.y - 12);

    graphics.DrawImage(&spaceship,static_cast<int>(position.x), static_cast<int>(position.y));

	if (engineOn) {
        Gdiplus::Image thrust(L"thrust.png");
        graphics.DrawImage(&thrust, static_cast<int>(position.x + 8), static_cast<int>(position.y + 27));
	}
}
