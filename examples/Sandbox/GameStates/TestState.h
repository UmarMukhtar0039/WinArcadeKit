#pragma once

#include "WinArcadeKit/GameState.h"

namespace wak {
	class Application;
}

class TestState : public wak::GameState
{
	virtual void OnUpdate(wak::Application& app, float deltaTime) override;
	virtual void OnRender(wak::Application& app) override;

private:
	float m_angle = 0.0f;
	float x = 640.0f;
	float y = 360.0f;
};