#include "WinArcadeKit/Graphics.h"

using Microsoft::WRL::ComPtr;

namespace wak
{
    Graphics::Graphics(HWND hwnd )
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = 0;
        swapChainDesc.BufferDesc.Height = 0;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;

        D3D11CreateDeviceAndSwapChain(
            NULL, // Default adapter
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG, // TODO: Should only in debug?
            NULL,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc, // NOTE: this is essentially the same as swapChainDesc.ReleaseAndGetAddressOf() which does release first.
            &m_swapChain,
            &m_device,
            NULL,
            &m_deviceContext
        );

        ComPtr<ID3D11Resource> backBuffer = nullptr;
        m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
        m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTarget);
    }
    
    void Graphics::BeginFrame()
    {
    }

    void Graphics::EndFrame()
    {
        m_swapChain->Present(1u, 0u);
    }

    void Graphics::ClearBuffer(float r, float g, float b, float a)
    {
        const float clearColor[4] = { r, g, b, a };
        m_deviceContext->ClearRenderTargetView(m_renderTarget.Get(), clearColor);
        m_deviceContext->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), NULL);
    }
}
