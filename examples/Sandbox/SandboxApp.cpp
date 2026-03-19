#include <WinArcadeKit/EntryPoint.h>
#include "GameStates/MainMenu.h"

wak::Application* wak::CreateApplication(int argc, char** argv)
{
    wak::AppSpec spec;
    spec.name = "🚀 テスト Sandbox 🎮";
	spec.width = 1280;
	spec.height = 720;

    auto* app = new Application(spec);
	app->RegisterState("MainMenu", new MainMenu());
	app->SetNextState("MainMenu");
    return app;
}
