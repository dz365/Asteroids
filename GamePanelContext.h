#pragma once
#include <windows.h>
#include "Panel.h"

class GamePanelContext
{
private:
	Panel panel;

public:
	GamePanelContext(Panel initPanel);
	void changePanel(Panel newPanel);
	void render(HWND hwnd);
};

