#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "GamePanelContext.h"
#include <memory>

class GamePanelContext;

enum class KeyAction {
	KEYDOWN,
	KEYUP
};

class Panel
{
protected:
	std::shared_ptr<GamePanelContext> context;
	HWND hwnd;

public:
	Panel(HWND hwnd);
	void setGamePanelContext(std::shared_ptr<GamePanelContext> newContext);
	virtual void render(Gdiplus::Graphics& graphics) = 0;
	virtual void handleKeyAction(KeyAction action, WPARAM key) = 0;
	virtual void handleTimerAction(UINT_PTR timerId) = 0;
	virtual void handleButtonClickedAction(UINT_PTR buttonId) = 0;
};

