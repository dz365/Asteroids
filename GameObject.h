#pragma once
#include <windows.h>

class GameObject
{
protected:
	POINT position;
	float velocity;

public:
	GameObject(POINT position, float velocity);

	POINT getPosition() const;
	void setPosition(POINT newPosition);

	float getVelocity();
	void setVelocity(float newVelocity);

	virtual RECT getBoundingRect() = 0;
	virtual void update() = 0;
	virtual void render(const HDC& hdc) = 0;
};

