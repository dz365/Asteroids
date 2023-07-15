#include "Player.h"

Player::Player() : GameObject({ 50, 50 }, 0) 
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
    RECT rect;
    rect.left = position.x - 10;
    rect.top = position.y;
    rect.right = position.x + 10;
    rect.bottom = position.y + 40;
    return rect;
}

void Player::update() {}

void Player::render(const HDC& hdc) 
{
	// Draw spaceship
	POINT vertices[5] = {
		{ position.x , position.y },
		{ position.x - 10, position.y + 30 },
		{ position.x - 3, position.y + 20 },
		{ position.x + 3, position.y + 20 },
		{ position.x + 10, position.y + 30 }
	};

	// Draw the polygon
	Polygon(hdc, vertices, 5);

	if (engineOn) {
        renderEngineFlames(hdc);
	}
}

void Player::renderEngineFlames(const HDC& hdc) {
    POINT leftFlameStart = { position.x - 5, position.y + 30 };
    POINT leftFlameEnd = { position.x, position.y + 40 };
    POINT rightFlameStart = { position.x + 5, position.y + 30 };
    POINT rightFlameEnd = { position.x, position.y + 40 };

    // Set the line color to red
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Draw left flame
    MoveToEx(hdc, leftFlameStart.x, leftFlameStart.y, nullptr);
    LineTo(hdc, leftFlameEnd.x, leftFlameEnd.y);

    // Draw right flame
    MoveToEx(hdc, rightFlameStart.x, rightFlameStart.y, nullptr);
    LineTo(hdc, rightFlameEnd.x, rightFlameEnd.y);

    // Clean up resources
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}