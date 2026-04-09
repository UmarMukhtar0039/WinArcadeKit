#pragma once

#include "WinArcadeKit/GameState.h"

namespace wak {
	class Application;
	class Texture;
}

class TestState : public wak::GameState
{
	virtual void OnActivate(wak::Application& app, wak::StateArgs& args) override;
	virtual void OnUpdate(wak::Application& app, float deltaTime) override;
	virtual void OnRender(wak::Application& app) override;
	virtual void OnDeactivate(wak::Application& app) override;

private:
	float m_angle = 0.0f;
	float x = 640.0f;
	float y = 360.0f;
	float m_fpsSmooth = 0.0f;
	wak::Texture* m_testTexture = nullptr;
};