#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "GamePanelContext.h"

class GamePanelContext;

enum class KeyAction {
	KEYDOWN,
	KEYUP
};

class Panel
{
protected:
	GamePanelContext* context;
	HWND hwnd;

public:
	Panel(HWND hwnd);
	void setGamePanelContext(GamePanelContext* newContext);
	virtual void render(Gdiplus::Graphics& graphics) = 0;
	virtual void handleKeyAction(KeyAction action, WPARAM key) = 0;
	virtual void handleTimerAction(UINT_PTR timerId) = 0;
};

