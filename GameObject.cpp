#include "GameObject.h"

GameObject::GameObject(POINT position, float velocity, float rotation, int health) 
	: position(position), velocity(velocity), rotation(rotation), health(health) {}

POINT GameObject::getPosition() const {
	return position;
}

void GameObject::setPosition(POINT newPosition) {
	position = newPosition;
}

float GameObject::getVelocity() {
	return velocity;
}

void GameObject::setVelocity(float newVelocity) {
	velocity = newVelocity;
}

float GameObject::getRotation() {
	return rotation;
}

void GameObject::setRotation(float newRotation) {
	rotation = newRotation;
}

int GameObject::getHealth() {
	return health;
}

void GameObject::update(const HWND& hWnd)
{
	// Update position
	position.x += static_cast<LONG>(sin(rotation) * velocity);
	position.y -= static_cast<LONG>(cos(rotation) * velocity);

	RECT rect;
	GetClientRect(hWnd, &rect);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	if (position.x < 0)
		position.x = windowWidth;
	if (position.x > windowWidth)
		position.x = 0;
	if (position.y < 0)
		position.y = windowHeight;
	if (position.y > windowHeight)
		position.y = 0;
}

void GameObject::handleCollision() {
	health--;
}