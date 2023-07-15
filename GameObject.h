#pragma once
#include <windows.h>

class GameObject
{
protected:
	POINT position;
	float velocity;
	float rotation; // in radians.

public:
	GameObject(POINT position, float velocity, float rotation);

	POINT getPosition() const;
	void setPosition(POINT newPosition);

	float getVelocity();
	void setVelocity(float newVelocity);

	float getRotation();
	void setRotation(float newRotation);

	virtual RECT getBoundingRect() = 0;
	virtual void update() = 0;
	virtual void render(const HDC& hdc) = 0;
};

