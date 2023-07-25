#pragma once
#include "GameObject.h"

class Player : public GameObject
{

private:
	bool engineOn;

public:
	Player();

	void startEngine();
	void stopEngine();
	RECT getBoundingRect();
	void update(const HWND& hWnd);
	void render(const HDC& hdc);
};

