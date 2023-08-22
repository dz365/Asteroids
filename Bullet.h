#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(POINT position, float velocity, float rotation, int health);
	RECT getBoundingRect();
	void render(Gdiplus::Graphics& graphics);
};

