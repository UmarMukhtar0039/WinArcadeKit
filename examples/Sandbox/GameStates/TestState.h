#pragma once

#include "WinArcadeKit/GameState.h"

namespace wak {
	class Application;
}

class TestState : public wak::GameState
{
	virtual void OnUpdate(wak::Application& app, float dt) override;

private:
	float m_angle = 0.016f;
	float x = 0.f;
	float y = 0.f;
};