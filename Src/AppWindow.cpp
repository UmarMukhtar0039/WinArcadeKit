#include "AppWindow.h"

const wchar_t* AppWindow::s_className = L"WinArcadeKit";
bool AppWindow::s_closeWindow = false;
bool AppWindow::s_lostFocus = false;

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

bool AppWindow::HasLostFocus()
{
    if (s_lostFocus)
    {
        s_lostFocus = false;
        return true;
    }
    return false;
}

void AppWindow::DestroyAppWindow()
{
    if (m_window)
    {
        DestroyWindow(m_window);
        m_window = nullptr;
    }
}

LRESULT CALLBACK AppWindow::MessageCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_CLOSE)
    {
		s_closeWindow = true; // handle close request in the message callback instead.
        return 0;
    }

    if(uMsg == WM_KILLFOCUS)
    {
        s_lostFocus = true;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}