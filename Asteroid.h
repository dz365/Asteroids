#pragma once
#include "GameObject.h"

class Asteroid : public GameObject
{

public:
	Asteroid(POINT position, float velocity, float rotation);
	RECT getBoundingRect();
	void update(const HWND& hWnd);
	void render(Gdiplus::Graphics& graphics);
};

