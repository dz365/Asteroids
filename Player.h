#pragma once
#include "GameObject.h"

class Player : public GameObject
{

private:
	bool engineOn;

	void renderEngineFlames(const HDC& hdc);

public:
	Player();

	void startEngine();
	void stopEngine();
	RECT getBoundingRect();
	void update();
	void render(const HDC& hdc);
};

