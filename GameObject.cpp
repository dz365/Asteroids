#include "GameObject.h"

GameObject::GameObject(POINT position, float velocity, float rotation) 
	: position(position), velocity(velocity), rotation(rotation) {}

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