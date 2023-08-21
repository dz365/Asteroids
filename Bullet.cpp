#include "Bullet.h"
#include <string>
Bullet::Bullet(POINT position, float velocity, float rotation)
	: GameObject(position, velocity, rotation)
{

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

void Bullet::render(Gdiplus::Graphics& graphics)
{
	Gdiplus::Image bullet(L"bullet.png"); // 4x6px

	graphics.TranslateTransform(position.x, position.y);
	graphics.RotateTransform(rotation * 180 / 3.14159265358979323846);
	graphics.TranslateTransform(-position.x, -position.y);
	graphics.DrawImage(
		&bullet, 
		static_cast<int>(position.x - 2), 
		static_cast<int>(position.y - 3)
	);
}
