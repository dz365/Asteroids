#pragma once
#include <windows.h>

class GameObject
{
private:
	POINT position;
	float velocity;

public:
	GameObject(POINT position, float velocity);

	POINT getPosition() const;
	void setPosition(POINT newPosition);

	float getVelocity();
	void setVelocity(float newVelocity);

	void update();
	void render();
};

