#pragma once
#include "GameObject.h"

enum Size {
	SMALL,
	MEDIUM,
	LARGE
};

class Asteroid : public GameObject
{
private:
	Size size;
public:
	Asteroid(POINT position, float velocity, float rotation, Size size);
	Size getSize();
	RECT getBoundingRect();
	void render(Gdiplus::Graphics& graphics);
};

