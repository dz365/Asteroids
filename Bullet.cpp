#include "Bullet.h"
#include <string>
Bullet::Bullet(POINT position, float velocity, float rotation)
	: GameObject(position, velocity, rotation)
{

}

RECT Bullet::getBoundingRect()
{
	return RECT();
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
