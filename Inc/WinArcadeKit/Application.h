#pragma once

class AppWindow;

namespace WinArcadeKit {

    class Application
    {
    public:
        Application();
        ~Application();

        void Run(void* hInstance);
    
    private:
        AppWindow *m_window;
    };

    // NOTE: to be implemneted by client
    Application* CreateApplication(int argc, char** argv);
}