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
    velocity -= 0.75f;
    if (velocity < 0)
        velocity = 0;
    if (velocity > 15)
        velocity = 15.0f;

    // Update position
    position.x += float(sin(rotation) * velocity);
    position.y -= float(cos(rotation) * velocity);

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

void rotatePoints(POINT points[], int pointsLength, POINT center, float rotation) {
    for (int i = 0; i < pointsLength; i++) {
        // Translate the vertex relative to the center
        int translatedX = points[i].x - center.x;
        int translatedY =points[i].y - center.y;

        // Apply the rotation transformation
        points[i].x = static_cast<int>(translatedX * cos(rotation) - translatedY * sin(rotation));
        points[i].y = static_cast<int>(translatedX * sin(rotation) + translatedY * cos(rotation));

        // Translate the vertex back to its original position
        points[i].x += center.x;
        points[i].y += center.y;
    }
}

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

    // Spaceship center
    POINT center = { position.x, position.y + 15 };
    rotatePoints(vertices, 5, center, rotation);
	Polygon(hdc, vertices, 5);

	if (engineOn) {
        renderEngineFlames(hdc);
	}
}

void Player::renderEngineFlames(const HDC& hdc) {
    POINT points[] = { 
        { position.x - 5, position.y + 30 }, 
        { position.x, position.y + 40 }, 
        { position.x + 5, position.y + 30 }, 
        { position.x, position.y + 40 } 
    };
    POINT center = { position.x, position.y + 15 };
    rotatePoints(points, 4, center, rotation);

    // Set the line color to red
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Draw left flame
    MoveToEx(hdc, points[0].x, points[0].y, nullptr);
    LineTo(hdc, points[1].x, points[1].y);

    // Draw right flame
    MoveToEx(hdc, points[2].x, points[2].y, nullptr);
    LineTo(hdc, points[3].x, points[3].y);

    // Clean up resources
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}