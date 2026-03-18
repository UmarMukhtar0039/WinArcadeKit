#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "AppWindow.h"

#include <Windows.h>

#ifdef _DEBUG
#include <iostream>
#endif

namespace wak {

    Application::Application(const AppSpec& appSpec)
        : m_appSpec(appSpec)
    {
        m_keyboard = new Keyboard();
    }

    Application::~Application()
    {
        delete m_window;
        m_window = nullptr;

		delete m_keyboard;
		m_keyboard = nullptr;
    }

    void Application::Run(void* hInstance)
    {
        m_window = new AppWindow(static_cast<HINSTANCE>(hInstance), m_appSpec.name, m_appSpec.width, m_appSpec.height);

        m_running = true;

        while(m_running)
        {
            PumpMessages();

            if (m_window->IsCloseRequested())
            {
                m_running = false;
            }

			m_keyboard->Update();

#ifdef _DEBUG
            if (m_keyboard->IsKeyPressed(VK_SPACE))
                std::cout << "SPACE pressed!" << std::endl;
            if (m_keyboard->IsKeyPressed('W'))
                std::cout << "W down" << std::endl;
            if (m_keyboard->IsKeyReleased('W'))
                std::cout << "W released" << std::endl;
#endif
        }
    }

    void Application::PumpMessages()
    {
        MSG msg = {}; // same as ZeroMemory(&msg, sizeof(msg));

        // need to drain all the messages in the queue each frame otherwise the app experience input lags.
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

