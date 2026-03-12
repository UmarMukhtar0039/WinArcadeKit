#include "WinArcadeKit/Application.h"

#include <Windows.h>

#include "AppWindow.h"

namespace wak {

    Application::Application(const AppSpec& appSpec)
        : m_appSpec(appSpec)
    {
    }

    Application::~Application()
    {
        delete m_window;
        m_window = nullptr;
    }

    void Application::Run(void* hInstance)
    {
        m_window = new AppWindow(static_cast<HINSTANCE>(hInstance), m_appSpec.name, m_appSpec.width, m_appSpec.height);

        m_running = true;

        while(m_running)
        {
            PumpMessages();
        }
    }

    void Application::PumpMessages()
    {
        MSG msg = {}; // same as ZeroMemory(&msg, sizeof(msg));

        // need to drain all the messages in the queue each frame otherwise the app experience input lags.
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            switch(msg.message)
            {
                case WM_QUIT:
                    m_running = false;
                    break;
                
                // Handle mouse and keyboard? Maybe not.
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

