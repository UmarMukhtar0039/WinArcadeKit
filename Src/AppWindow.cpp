#include "AppWindow.h"

const char* AppWindow::s_className = TEXT("WinArcadeKit");

AppWindow::AppWindow(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
    RegisterWindowClass();
    CreateAppWindow();
}

AppWindow::~AppWindow()
{
    DestroyAppWindow();
    UnregisterWindowClass();
}

void AppWindow::RegisterWindowClass()
{
    WNDCLASSEX classDesc;
    ZeroMemory(&classDesc, sizeof(classDesc));

    classDesc.cbSize = sizeof(classDesc);
    classDesc.style = CS_HREDRAW | CS_VREDRAW;
    classDesc.lpfnWndProc = &MessageCallback;
    classDesc.hInstance = m_hInstance;
    classDesc.hIcon = nullptr; // TODO: might wanna revisit this
    classDesc.hIconSm = nullptr; //TODO: Might wanna revisit this for taskbar icon. Should probably pass this via some AppSpec.
    classDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
    classDesc.lpszClassName = s_className;

    m_windowClass = RegisterClassEx(&classDesc);
}

void AppWindow::UnregisterWindowClass()
{
    UnregisterClass(s_className, m_hInstance);
}

void AppWindow::CreateAppWindow()
{
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    DWORD windowStyleEx = WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;

    // TODO: this should be passed in via some AppSpec or something, not hardcoded here
    RECT clientSize;
    ZeroMemory(&clientSize, sizeof(clientSize));
    clientSize.right = 1280;
    clientSize.bottom = 720;

    AdjustWindowRectEx(&clientSize, windowStyle, FALSE, windowStyleEx);

    m_window = CreateWindowEx(
        windowStyleEx,
        s_className,
        TEXT("WinArcadeKit App"), // This should be set by the app and passed in via AppSpec
        windowStyle | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        clientSize.right - clientSize.left, // total width
        clientSize.bottom - clientSize.top, // total height
        nullptr, nullptr, m_hInstance, nullptr);
}

void AppWindow::DestroyAppWindow()
{
    DestroyWindow(m_window);
}

LRESULT CALLBACK AppWindow::MessageCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}