#pragma once
#include "GameObject.h"

enum Size {
	LARGE = 60,
	MEDIUM = 45,
	SMALL = 30
};

class Asteroid : public GameObject
{
private:
	Size size;
public:
	Asteroid(POINT position, float velocity, float rotation, int health, Size size);
	RECT getBoundingRect();
	void render(Gdiplus::Graphics& graphics);
};

