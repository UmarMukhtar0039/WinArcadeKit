#pragma once

class AppWindow;

namespace wak {

    class Application
    {
    public:
        Application();
        ~Application();

        void Run(void* hInstance);
    
    private:
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
    
    private:
        AppWindow *m_window;
    };

    // NOTE: to be implemneted by client
    Application* CreateApplication(int argc, char** argv);
}