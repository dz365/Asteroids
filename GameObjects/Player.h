#pragma once
#include "GameObject.h"

class Player : public GameObject
{

private:
	bool engineOn;
	int bulletsAvailable;
	float heading;				// Direction the player is moving


public:
	Player();

	void startEngine();
	void stopEngine();
	void increaseBullets();
	void decreaseBullets();
	int getBulletsAvailable();
	RECT getBoundingRect();
	void update(const HWND& hWnd) override;
	void render(Gdiplus::Graphics& graphics);
	void handleCollision() override;
};

