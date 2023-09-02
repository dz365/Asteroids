#include "Panel.h"

Panel::Panel(HWND hwnd) : hwnd(hwnd), context(nullptr) {}

void Panel::setGamePanelContext(std::shared_ptr<GamePanelContext> newContext)
{
	context = newContext;
}

