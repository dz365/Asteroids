#include "Player.h"

Player::Player() : GameObject({ 50, 50 }, 0) {}

void Player::update() {}

void Player::render(const HDC& hdc) 
{
	//Define the polygon vertices
	int x = 50;
	int y = 50;
	POINT vertices[5] = {
		{ x , y },
		{ x - 10, y + 30 },
		{ x - 3, y + 20 },
		{ x + 3, y + 20 },
		{ x + 10, y + 30 }
	};

	// Draw the polygon
	Polygon(hdc, vertices, 5);
}