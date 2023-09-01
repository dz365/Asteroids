#pragma once
#include "Panel.h"
#include <memory>
#include "GameObject.h"
#include "Player.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Alien.h"
#include <list>
#include <unordered_map>
#include <unordered_set>

class PlayingPanel : public Panel
{
private:
	// GAMEOBJECTS
	std::shared_ptr<Player> player;
	std::list<std::shared_ptr<Asteroid>> asteroidObjects;
	std::list<std::shared_ptr<Bullet>> bulletObjects;
	std::list<std::shared_ptr<Alien>> alienObjects;
	std::list<std::shared_ptr<Bullet>> alienBulletObjects;
	std::unordered_map<UINT_PTR, std::shared_ptr<Alien>> alienTimers;

	std::unordered_set<WPARAM> pressedKeys;

	int numAliensGenerated;
	int score;
	boolean allowedToFire;

	void generateAsteroid();
	void generateAlien();
	void generateAlienBullet(int alienId);

	void renderScore(Gdiplus::Graphics& graphics);
	void renderPlayerHealth(Gdiplus::Graphics& graphics);
	void renderPlayerRemainingBullets(Gdiplus::Graphics& graphics);
	void renderGameObject(Gdiplus::Graphics& graphics, std::shared_ptr<GameObject> obj);
	bool hasCollided(RECT r1, RECT r2);

	void handlePlayerCollision();
	void handlePlayerBulletCollision();
	void handleAsteroidFragmentation(std::shared_ptr<Asteroid> asteroid);
	void handleAlienBulletCollision();
	void handleOutOfBoundBullets();

	void handlePressedKeys();
	void update();

	void gameOver();
public:
	PlayingPanel(HWND hwnd);
	void render(Gdiplus::Graphics& graphics);
	void handleKeyAction(KeyAction action, WPARAM key);
	void handleTimerAction(UINT_PTR timerId);
	void handleButtonClickedAction(UINT_PTR buttonId);
};

