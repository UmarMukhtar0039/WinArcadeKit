#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace wak
{
    class Graphics
    {
    public:
        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;
    
        static Graphics* CreateDevice(HWND hwnd);
        static void DestroyDevice(Graphics* device);
    
        void EndFrame();
        void BeginFrame();
        void ClearBuffer(float r, float g, float b, float a);

	    void DrawTestTriangle(); // TODO: remove this after testing.

    private:
        Graphics() = default;
        ~Graphics() = default; // NOTE: we are using ComPtr, so we don't need to manually release resources.
    
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;
    };

}