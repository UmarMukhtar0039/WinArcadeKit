#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <memory>

#include "GameState.h"

class AppWindow;

namespace wak {

    class Graphics;
    class Keyboard;

    struct AppSpec
    {
        std::string name = "WinArcadeKit App";
        uint32_t width = 1280;
        uint32_t height = 720;
    };

    class Application
    {
    public:
        Application(const AppSpec& appSpec = AppSpec());
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void Run(void* hInstance);

        Graphics& GetGraphics() const { return *m_graphics; }

		Keyboard* GetKeyboard() const { return m_keyboard; }

        // TODO: We might wanna move this to a separate StateManager class in the future if it gets more complex, but for now we'll just keep it here.
        /* State Management*/
        void RegisterState(std::string_view name, GameState* state);
        void SetNextState(std::string_view name, StateArgs args = {});
        /******************/

    private:
        void PumpMessages();
    
        void SwapState();

    private:
        AppWindow* m_window = nullptr;
        AppSpec m_appSpec;

        // Graphics
        std::unique_ptr<Graphics> m_graphics;

        // Inputs
		Keyboard* m_keyboard = nullptr;

        bool m_running = false;

		// State management
        std::map<std::string, GameState*, std::less<>> m_gameStates;
        GameState* m_currentState = nullptr;
		GameState* m_nextState = nullptr;
		StateArgs m_nextStateArgs;
    };

    // NOTE: to be implemneted by client
    Application* CreateApplication(int argc, char** argv);
}