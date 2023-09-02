#pragma once
#include "Panel.h"
#include <list>
#include "../GameObjects/GameObject.h"
class StartMenuPanel : public Panel
{
private:
	HWND NEWGAME_BUTTON;
	static const int NEWGAME_BUTTON_ID = 1;
	static const UINT_PTR RENDER_ID = 1;
public:
	StartMenuPanel(HWND hwnd);
	void render(Gdiplus::Graphics& graphics);
	void handleKeyAction(KeyAction action, WPARAM key);
	void handleTimerAction(UINT_PTR timerId);
	void handleButtonClickedAction(UINT_PTR buttonId);
};

