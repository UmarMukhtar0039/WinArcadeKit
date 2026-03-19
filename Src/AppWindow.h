#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>


class AppWindow
{
public:
    AppWindow(HINSTANCE hInstance, const std::string& title, uint32_t width, uint32_t height);
    ~AppWindow();

    AppWindow(const AppWindow &) = delete;
    void operator=(const AppWindow &) = delete;

	bool IsCloseRequested() const { return s_closeWindow; }
	bool HasLostFocus();

private:
    void RegisterWindowClass();
    void UnregisterWindowClass();
    void CreateAppWindow();
    void DestroyAppWindow();

    static LRESULT CALLBACK MessageCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    ATOM m_windowClass = 0;
    HWND m_window = nullptr;
    HINSTANCE m_hInstance = nullptr;
	
    static const wchar_t* s_className;
	static bool s_closeWindow;
	static bool s_lostFocus;

	std::wstring m_title;
	uint32_t m_width;
	uint32_t m_height;
};
