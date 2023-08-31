#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "Panel.h"

class Panel;

class GamePanelContext
{
private:
	Panel* panel;

public:
	GamePanelContext(Panel* initPanel);
	void changePanel(Panel* newPanel);
	void render(Gdiplus::Graphics& graphics);
};

