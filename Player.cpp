#include "Player.h"

Player::Player() : GameObject({ 150, 150 }, 0, 0, 3) 
{
    engineOn = false;
    bulletsAvailable = 5;
}

void Player::startEngine() {
	engineOn = true;
}

void Player::stopEngine() {
    engineOn = false;
}

void Player::increaseBullets()
{
    bulletsAvailable++;
}

void Player::decreaseBullets()
{
    bulletsAvailable--;
}

int Player::getBulletsAvailable()
{
    return bulletsAvailable;
}

RECT Player::getBoundingRect() {
    RECT rect{};
    rect.left = position.x - 12;
    rect.top = position.y - 12;
    rect.right = position.x + 12;
    rect.bottom = position.y + 12;
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

    GameObject::update(hWnd);
}

void Player::render(Gdiplus::Graphics& graphics)
{
    Gdiplus::Image spaceship(L"spaceship.png"); // 24x24px

    graphics.TranslateTransform(position.x, position.y);
    graphics.RotateTransform(rotation * 180 / 3.14159265358979323846);
    graphics.TranslateTransform(-position.x, -position.y);

    graphics.DrawImage(
        &spaceship,
        static_cast<int>(position.x - 12), 
        static_cast<int>(position.y - 12)
    );

	if (engineOn) {
        Gdiplus::Image thrust(L"thrust.png");
        graphics.DrawImage(
            &thrust, 
            static_cast<int>(position.x - 9), 
            static_cast<int>(position.y + 15)
        );
	}
}

