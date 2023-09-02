#pragma once
#include "GameObject.h"

class Alien : public GameObject
{
private:
	int id;
public:
	Alien(POINT position, int id);
	int getId();
	RECT getBoundingRect();
	void render(Gdiplus::Graphics& graphics);
};

