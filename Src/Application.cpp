#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "AppWindow.h"

#include <Windows.h>
#include <cassert>

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
        for (auto& [name, state] : m_gameStates)
            delete state;
        m_gameStates.clear();

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
            SwapState();

            // TODO: put all this in Update?
            if (m_window->IsCloseRequested())
            {
                m_running = false;
            }

            if (m_window->HasLostFocus())
            {
                m_keyboard->Reset();
            }

			m_keyboard->Update();
            m_currentState->OnUpdate(*this, 0.0f); // TODO: Need timestep
        }

        if (m_currentState)
        {
            m_currentState->OnDeactivate(*this);
        }
    }

    void Application::PumpMessages()
    {
        MSG msg = {}; // same as ZeroMemory(&msg, sizeof(msg));

        // need to drain all the messages in the queue each frame otherwise the app experience input lags.
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
			// TODO: Not using WM_CHAR messages for now, but we might need to in the future for text input. If we do, we'll need to translate the message here.
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	// TODO: We might wanna move this to a separate StateManager class in the future if it gets more complex, but for now we'll just keep it here.
#pragma region StateManagement

    void Application::SwapState()
    {
        if (m_nextState)
        {
            if (m_currentState)
            {
                m_currentState->OnDeactivate(*this);
            }

            m_currentState = m_nextState;
            m_nextState = nullptr;
            m_currentState->OnActivate(*this, m_nextStateArgs);
            m_nextStateArgs.clear();
        }
    }

    void Application::RegisterState(std::string_view name, GameState* state)
    {
        auto [it, isInserted] = m_gameStates.emplace(std::string(name), state);
        assert(isInserted && "State with this name already registered!");
    }
    
    void Application::SetNextState(std::string_view name, StateArgs args)
    {
		auto it = m_gameStates.find(name);
		assert(it != m_gameStates.end() && "State not found! Was it registered?");
		m_nextState = it->second;
		m_nextStateArgs = std::move(args);
	}

#pragma endregion StateManagement

}


