#include "GamePanelContext.h"

GamePanelContext::GamePanelContext(Panel* initPanel) 
	: panel(initPanel) {}

void GamePanelContext::changePanel(Panel* newPanel)
{
	panel = newPanel;
}

void GamePanelContext::render(Gdiplus::Graphics& graphics)
{
	panel->render(graphics);
}