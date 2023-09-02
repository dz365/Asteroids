#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
private:
	float distanceTravelled;
public:
	Bullet(POINT position, float velocity, float rotation, int health);

	float getDistanceTravelled();

	RECT getBoundingRect();
	void update(const HWND& hWnd) override;
	void render(Gdiplus::Graphics& graphics);
};

