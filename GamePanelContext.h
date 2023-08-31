#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "Panel.h"
#include <memory>

enum class KeyAction;
class Panel;

class GamePanelContext
{
private:
	std::shared_ptr<Panel> panel;

public:
	GamePanelContext(std::shared_ptr<Panel> initPanel);
	void changePanel(std::shared_ptr<Panel> newPanel);
	void render(Gdiplus::Graphics& graphics);
	void handleKeyAction(KeyAction action, WPARAM key);
	void handleTimerAction(UINT_PTR timerId);
};

