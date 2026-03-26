#include <WinArcadeKit/EntryPoint.h>
#include "GameStates/TestState.h"

wak::Application* wak::CreateApplication(int argc, char** argv)
{
    wak::AppSpec spec;
    spec.name = "🚀 テスト Sandbox 🎮";
	spec.width = 1280;
	spec.height = 720;

    auto* app = new Application(spec);
	app->RegisterState("TestState", new TestState());
	app->SetNextState("TestState");
    return app;
}
