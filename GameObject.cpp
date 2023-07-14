#include "GameObject.h"

GameObject::GameObject(POINT position, float velocity) 
	: position(position), velocity(velocity) {}

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