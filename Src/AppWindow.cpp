#include "AppWindow.h"

const wchar_t* AppWindow::s_className = L"WinArcadeKit";

static std::wstring Utf8ToWide(const std::string& str)
{
    if (str.empty()) return {};
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), result.data(), size);
    return result;
}

AppWindow::AppWindow(HINSTANCE hInstance, const std::string& title, uint32_t width, uint32_t height)
	: m_hInstance(hInstance), m_title(Utf8ToWide(title)), m_width(width), m_height(height)
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
    clientSize.right = m_width;
    clientSize.bottom = m_height;

    AdjustWindowRectEx(&clientSize, windowStyle, FALSE, windowStyleEx);

    m_window = CreateWindowEx(
        windowStyleEx,
        s_className,
        m_title.c_str(),
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