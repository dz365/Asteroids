#pragma once
#include "Panel.h"
#include <list>
#include "./GameObjects/GameObject.h"
class GameOverPanel : public Panel
{
private:
	int score;
	std::list<std::shared_ptr<GameObject>> gameObjects;
	HWND NEWGAME_BUTTON;
	HWND STARTMENU_BUTTON;
	static const int NEWGAME_BUTTON_ID = 1;
	static const int STARTMENU_BUTTON_ID = 2;
public:
	GameOverPanel(HWND hwnd, std::list<std::shared_ptr<GameObject>> gameObjects, int score);
	void render(Gdiplus::Graphics& graphics);
	void handleKeyAction(KeyAction action, WPARAM key);
	void handleTimerAction(UINT_PTR timerId);
	void handleButtonClickedAction(UINT_PTR buttonId);
};

