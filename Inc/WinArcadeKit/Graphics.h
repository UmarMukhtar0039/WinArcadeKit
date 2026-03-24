#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace wak
{
    class Graphics
    {
    public:
        Graphics(HWND hwnd );
        ~Graphics() = default; // NOTE: we are using ComPtr, so we don't need to manually release resources.

        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;

        void EndFrame();
        void BeginFrame();
        void ClearBuffer(float r, float g, float b, float a);
        
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;
    };

}