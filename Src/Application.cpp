#include "WinArcadeKit/Application.h"

#include <Windows.h>

#include "AppWindow.h"

namespace wak {

    Application::Application()
    {
    }

    Application::~Application()
    {
        delete m_window;
        m_window = nullptr;
    }

    void Application::Run(void* hInstance)
    {
        m_window = new AppWindow(static_cast<HINSTANCE>(hInstance));

        MSG msg = {};
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}