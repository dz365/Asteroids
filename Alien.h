#pragma once
#include "GameObject.h"

class Alien : public GameObject
{
public:
	Alien(POINT position, float velocity, float rotation, int health);
	RECT getBoundingRect();
	void render(Gdiplus::Graphics& graphics);
};

