#pragma once

#include <Windows.h>

#include "Application.h"

extern WinArcadeKit::Application* WinArcadeKit::CreateApplication(int argc, char** argv);

namespace WinArcadeKit {

    int Main(HINSTANCE hInstance, int argc, char** argv)
    {
        Application* app = CreateApplication(argc, argv);
        app->Run(hInstance);
        return 0;
    }

}

#ifdef WAK_DIST

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    return WinArcadeKit::Main(hInstance, __argc, __argv);
}

#else

int main(int argc, char** argv)
{
    return WinArcadeKit::Main(GetModuleHandle(NULL), argc, argv);
}

#endif // WAK_DIST