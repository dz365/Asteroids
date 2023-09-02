#pragma once
#include <windows.h>
#include <cmath>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class GameObject
{
protected:
	POINT position; // center
	float velocity; // x km/s
	float rotation; // in radians.
	int health;

public:
	GameObject(POINT position, float velocity, float rotation, int health);

	POINT getPosition() const;
	void setPosition(POINT newPosition);

	float getVelocity();
	void setVelocity(float newVelocity);

	float getRotation();
	void setRotation(float newRotation);

	int getHealth();

	virtual RECT getBoundingRect() = 0;
	virtual void update(const HWND& hWnd);
	virtual void render(Gdiplus::Graphics& graphics) = 0;
	virtual void handleCollision();
};

