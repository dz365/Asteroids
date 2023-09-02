#include "GamePanelContext.h"

GamePanelContext::GamePanelContext(std::shared_ptr<Panel> initPanel)
	: panel(initPanel) 
{
}

void GamePanelContext::changePanel(std::shared_ptr<Panel> newPanel)
{
	panel = newPanel;
}

void GamePanelContext::render(Gdiplus::Graphics& graphics)
{
	panel->render(graphics);
}

void GamePanelContext::handleKeyAction(KeyAction action, WPARAM key)
{
	panel->handleKeyAction(action, key);
}

void GamePanelContext::handleTimerAction(UINT_PTR timerId)
{
	panel->handleTimerAction(timerId);
}

void GamePanelContext::handleButtonClickedAction(UINT_PTR buttonId)
{
	panel->handleButtonClickedAction(buttonId);
}
