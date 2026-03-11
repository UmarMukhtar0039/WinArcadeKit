#include <WinArcadeKit/EntryPoint.h>

wak::Application* wak::CreateApplication(int argc, char** argv)
{
    wak::AppSpec spec;
    spec.name = "🚀 テスト Sandbox 🎮";
    return new Application(spec);
}
