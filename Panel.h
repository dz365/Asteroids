#pragma once
#include <windows.h>
#include "GamePanelContext.h"
class Panel
{
private:
	GamePanelContext context;

public:
	void setGamePanelContext(GamePanelContext newContext);
	virtual void render(HWND hwnd);
};

