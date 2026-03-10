#pragma once

#include <Windows.h>
#include <cstdint>

class AppWindow
{
public:
    AppWindow(HINSTANCE hInstance);
    ~AppWindow();

private:
    AppWindow(const AppWindow &) = delete;
    void operator=(const AppWindow &) = delete;

    void RegisterWindowClass();
    void UnregisterWindowClass();
    void CreateAppWindow();
    void DestroyAppWindow();

    static LRESULT CALLBACK MessageCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    ATOM m_windowClass = 0;
    HWND m_window = nullptr;
    HINSTANCE m_hInstance = nullptr;
	
    static const char* s_className;
};
