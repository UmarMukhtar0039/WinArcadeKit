#include <WinArcadeKit/EntryPoint.h>

WinArcadeKit::Application* WinArcadeKit::CreateApplication(int argc, char** argv)
{
    return new Application();
}
