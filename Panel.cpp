#include "Panel.h"

Panel::Panel(HWND hwnd) : hwnd(hwnd), context(nullptr) {}

void Panel::setGamePanelContext(GamePanelContext* newContext)
{
	context = newContext;
}

