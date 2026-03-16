#pragma once

#include <string>
#include <cstdint>

class AppWindow;

namespace wak {

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

        void Run(void* hInstance);
    
    private:
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void PumpMessages();
    
    private:
        AppWindow* m_window = nullptr;
        AppSpec m_appSpec;

        bool m_running = false;
    };

    // NOTE: to be implemneted by client
    Application* CreateApplication(int argc, char** argv);
}