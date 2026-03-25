#include "WinArcadeKit/Graphics.h"

#include <memory>

using Microsoft::WRL::ComPtr;

namespace wak
{
    Graphics::Graphics()
    {
    }

    Graphics* Graphics::CreateDevice(HWND hwnd)
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

        UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  #ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
  #endif

        ComPtr<IDXGISwapChain> dxgiSwapChain;
        ComPtr<ID3D11Device> d3dDevice;
        ComPtr<ID3D11DeviceContext> d3dDeviceContext;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            NULL, // Default adapter
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            flags,
            NULL,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc, 
            &dxgiSwapChain, // NOTE: this is essentially the same as dxgiSwapChain.ReleaseAndGetAddressOf() which does release first.
            &d3dDevice,
            NULL,
            &d3dDeviceContext
        );

        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Failed to create D3D11 device.", L"WinArcadeKit Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        ComPtr<ID3D11Resource> backBuffer = nullptr;
        hr = dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Failed to get swap chain buffer.", L"WinArcadeKit Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        ComPtr<ID3D11RenderTargetView> renderTarget = nullptr;
        hr = d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Failed to create render target view.", L"WinArcadeKit Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        Graphics* graphics = new Graphics();
        graphics->m_device = std::move(d3dDevice);
        graphics->m_deviceContext = std::move(d3dDeviceContext);
        graphics->m_swapChain = std::move(dxgiSwapChain);
        graphics->m_renderTarget = std::move(renderTarget);
        return graphics;
    }

    void Graphics::DestroyDevice(Graphics* device)
    {
        delete device;
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
