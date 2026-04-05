#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "WinArcadeKit/Graphics.h"
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
        
        Graphics::DestroyDevice(m_graphics);
        m_graphics = nullptr;
    }

    void Application::Run(void* hInstance)
    {
        m_window = new AppWindow(static_cast<HINSTANCE>(hInstance), m_appSpec.name, m_appSpec.width, m_appSpec.height);
        m_graphics = wak::Graphics::CreateDevice(m_window->GetHandle(), m_appSpec.width, m_appSpec.height);

        if (!m_graphics)
        {
            return;
        }

        if (!m_nextState)
        {
            MessageBox(nullptr, L"No initial state set! Call SetNextState() before Run().", L"WinArcadeKit Error", MB_OK | MB_ICONERROR);
            return;
        }

        m_running = true;

        // Initialize QPC timing
        LARGE_INTEGER freq, now;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&now);
        m_qpcFrequency = freq.QuadPart;
        m_qpcPrevious = now.QuadPart;

        while(m_running)
        {
            PumpMessages();
            SwapState();
			Update();
            Render();
        }

        if (m_currentState)
        {
            m_currentState->OnDeactivate(*this);
        }
    }

    void Application::Update()
    {
        if (m_window->IsCloseRequested())
        {
            m_running = false;
        }

        // TODO: doesn't do shit honestly check Keyboard.h comment at the top
        if (m_window->HasLostFocus())
        {
            m_keyboard->Reset();
        }

        // Compute delta time via QPC
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        float rawDt = static_cast<float>(
            static_cast<double>(now.QuadPart - m_qpcPrevious) /
            static_cast<double>(m_qpcFrequency)
        );

        m_qpcPrevious = now.QuadPart;

        // Clamp to prevent spiral of death
        // TODO: we might wanna discard the frame if dt is too large bcz right now the objects would teleport when we drag the game window and then stop dragging. i.e just set rawDT to 0
        if (rawDt > MAX_DELTA_TIME)
            rawDt = MAX_DELTA_TIME;

        // Update Time struct
        m_time.unscaledDeltaTime = rawDt;
        m_time.deltaTime = rawDt * m_time.timeScale;
        m_time.timeElapsed += static_cast<double>(rawDt);
        m_time.frameCount++;

        m_keyboard->Update();
        m_currentState->OnUpdate(*this, m_time); // TODO: We migt pass in deltaTime only and rest should be accessed via app.GetTime() as this might be consfusing for others.
    }

    const Time& Application::GetTime() const
    {
        return m_time;
    }

    void Application::SetTimeScale(float scale)
    {
        m_time.timeScale = scale;
    }

    void Application::Render()
    {
        m_graphics->BeginFrame();

        if (m_currentState)
        {
            m_currentState->OnRender(*this);
        }

        m_graphics->EndFrame();
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


