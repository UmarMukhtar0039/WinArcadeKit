#pragma once

namespace WinArcadeKit {

    int Main(HINSTANCE hInstance, int argc, char** argv)
    {
        return 0;
    }

}

#ifdef WAK_DIST
#include <Windows.h>

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