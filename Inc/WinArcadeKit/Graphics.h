#pragma once

#include <Windows.h>
#include <d3d11.h>


namespace wak
{
    class Graphics
    {
    public:
        Graphics(HWND hwnd );
        ~Graphics();

        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;

        void EndFrame();
        void BeginFrame();
        void ClearBuffer(float r, float g, float b, float a);
        
    private:
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_deviceContext = nullptr;
        IDXGISwapChain* m_swapChain = nullptr;
        ID3D11RenderTargetView* m_renderTarget = nullptr;
    };

}