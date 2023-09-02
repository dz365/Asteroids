#include "Bullet.h"
#include <string>
Bullet::Bullet(POINT position, float velocity, float rotation, int health)
	: GameObject(position, velocity, rotation, health)
{
	distanceTravelled = 0;
}

float Bullet::getDistanceTravelled() {
	return distanceTravelled;
}

RECT Bullet::getBoundingRect()
{
	RECT rect{};
	rect.left = position.x - 3;
	rect.top = position.y - 3;
	rect.right = position.x + 3;
	rect.bottom = position.y + 3;
	return rect;
}

void Bullet::update(const HWND& hWnd)
{
	distanceTravelled += sqrt(
		pow(sin(rotation) * velocity, 2) + pow(cos(rotation) * velocity, 2)
	);

	GameObject::update(hWnd);
}

void Bullet::render(Gdiplus::Graphics& graphics)
{
	Gdiplus::Image bullet(L"Resources\\bullet.png"); // 4x6px

	graphics.TranslateTransform(position.x, position.y);
	graphics.RotateTransform(rotation * 180 / 3.14159265358979323846);
	graphics.TranslateTransform(-position.x, -position.y);
	graphics.DrawImage(
		&bullet, 
		static_cast<int>(position.x - 2), 
		static_cast<int>(position.y - 3)
	);
}
