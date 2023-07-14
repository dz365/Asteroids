#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void update();
	void render(const HDC& hdc);
};

