#pragma once

#include "WinArcadeKit/GameState.h"

namespace wak {
	class Application;
}

class MainMenu : public wak::GameState
{
	virtual void OnUpdate(wak::Application& app, float dt) override;

};