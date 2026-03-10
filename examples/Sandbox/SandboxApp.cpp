#include <WinArcadeKit/EntryPoint.h>

wak::Application* wak::CreateApplication(int argc, char** argv)
{
    return new Application();
}
