#include "Player.h"

Player::Player() : 
    GameObject({ 250, 250 }, 0, 0, 3), 
    engineOn(false), 
    bulletsAvailable(5),
    heading(0){}

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
    if (engineOn) {
        if (abs(heading - rotation) > 3.14 / 2)
            velocity *= 0.1;
        heading = rotation;
        velocity = min(12.0f, velocity + 0.25);
    }
    else {
        velocity = max(0, velocity - 0.1f);
    }

    // Update position
    position.x += static_cast<LONG>(sin(heading) * velocity);
    position.y -= static_cast<LONG>(cos(heading) * velocity);

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

void Player::render(Gdiplus::Graphics& graphics)
{
    Gdiplus::Image spaceship(L"Resources\\spaceship.png"); // 24x24px

    graphics.TranslateTransform(position.x, position.y);
    graphics.RotateTransform(rotation * 180 / 3.14159265358979323846);
    graphics.TranslateTransform(-position.x, -position.y);

    graphics.DrawImage(
        &spaceship,
        static_cast<int>(position.x - 12), 
        static_cast<int>(position.y - 12)
    );

	if (engineOn) {
        Gdiplus::Image thrust(L"Resources\\thrust.png");
        graphics.DrawImage(
            &thrust, 
            static_cast<int>(position.x - 9), 
            static_cast<int>(position.y + 15)
        );
	}
}

void Player::handleCollision() {
    GameObject::handleCollision();
    position = { 250, 250 };
}