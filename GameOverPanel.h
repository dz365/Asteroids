#pragma once
#include "Panel.h"
#include <list>
#include "GameObject.h"
class GameOverPanel : public Panel
{
private:
	int score;
	std::list<std::shared_ptr<GameObject>> gameObjects;
public:
	GameOverPanel(HWND hwnd, std::list<std::shared_ptr<GameObject>> gameObjects, int score);
	void render(Gdiplus::Graphics& graphics);
	void handleKeyAction(KeyAction action, WPARAM key);
	void handleTimerAction(UINT_PTR timerId);
	void handleButtonClickedAction(UINT_PTR buttonId);
};

