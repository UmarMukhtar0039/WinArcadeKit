#pragma once

#include <Windows.h>

#include "Application.h"

extern wak::Application* wak::CreateApplication(int argc, char** argv);

namespace wak {

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
    return wak::Main(hInstance, __argc, __argv);
}

#else

int main(int argc, char** argv)
{
    return wak::Main(GetModuleHandle(NULL), argc, argv);
}

#endif // WAK_DIST