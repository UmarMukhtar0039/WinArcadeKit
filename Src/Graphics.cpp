#include "WinArcadeKit/Graphics.h"
#include "WinArcadeKit/Texture.h"

#include <cmath>
#include <iterator>
#include <DirectXMath.h>

#include "WinArcadeKit/Vertex.h"
#include "ImmediateMode.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace wak
{
    Graphics* Graphics::CreateDevice(HWND hwnd, FLOAT width, FLOAT height)
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

        ComPtr<ID3D11Resource> backBuffer;
        hr = dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Failed to get swap chain buffer.", L"WinArcadeKit Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        ComPtr<ID3D11RenderTargetView> renderTarget;
        hr = d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Failed to create render target view.", L"WinArcadeKit Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        // enable alpha blending
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        ComPtr<ID3D11BlendState> blendState;
        d3dDevice->CreateBlendState(&blendDesc, &blendState);
        d3dDeviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

        // Disable backface culling nobody uses culling in 2D games.
        D3D11_RASTERIZER_DESC rasterDesc = {};
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_NONE;
        ComPtr<ID3D11RasterizerState> rasterState;
        d3dDevice->CreateRasterizerState(&rasterDesc, &rasterState);
        d3dDeviceContext->RSSetState(rasterState.Get());

        // Initialize COM for WIC
        HRESULT hrCom = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hrCom) && hrCom != S_FALSE && hrCom != RPC_E_CHANGED_MODE)
            return nullptr;

        // Create WIC factory for texture loading
        ComPtr<IWICImagingFactory> wicFactory;
        HRESULT hrWic = CoCreateInstance(
            CLSID_WICImagingFactory, nullptr,
            CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
        if (FAILED(hrWic))
            return nullptr;

		ImmediateMode* immediateMode = ImmediateMode::Create(d3dDevice.Get(), d3dDeviceContext.Get(), width, height);
        if (!immediateMode)
        {
            MessageBox(nullptr, L"Failed to create ImmediateMode renderer.", L"WinArcadeKit Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        Graphics* graphics = new Graphics();
        graphics->m_device = std::move(d3dDevice);
        graphics->m_deviceContext = std::move(d3dDeviceContext);
        graphics->m_swapChain = std::move(dxgiSwapChain);
        graphics->m_renderTarget = std::move(renderTarget);
		graphics->m_immediateMode = immediateMode;
		graphics->m_wicFactory = std::move(wicFactory);
		graphics->m_width = width;
		graphics->m_height = height;
		return graphics; // TODO: wrap this in a unique_ptr once we decide on the ownership model for Graphics.
    }

    void Graphics::DestroyDevice(Graphics* gfx)
    {
		ImmediateMode::Destroy(gfx->m_immediateMode);
		gfx->m_immediateMode = nullptr;

        gfx->m_wicFactory.Reset();
        CoUninitialize();

        delete gfx;
    }
    
    void Graphics::BeginFrame()
    {
        ClearBuffer(0.39f, 0.58f, 0.93f, 1.0f); // Cornflower blue

        D3D11_VIEWPORT viewport = {};
        viewport.Width = m_width;
        viewport.Height = m_height;
        viewport.MaxDepth = 1.0f;

        m_deviceContext->RSSetViewports(1u, &viewport); // rasterizer stage viewport setup
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

    void Graphics::Draw(D3D11_PRIMITIVE_TOPOLOGY topology, const Vertex* vertices, unsigned int count)
    {
		m_immediateMode->Draw(topology, vertices, count);
    }

    void Graphics::SetModelMatrix(XMMATRIX transform)
    {
        m_immediateMode->SetModelMatrix(transform);
    }

    void Graphics::SetViewMatrix(XMMATRIX viewMatrix)
    {
        m_immediateMode->SetViewMatrix(viewMatrix);
    }

    void Graphics::SetProjectionMatrix(XMMATRIX projectionMatrix)
    {
        m_immediateMode->SetProjectionMatrix(projectionMatrix);
    }

    Texture* Graphics::LoadTexture(const wchar_t* filename)
    {
        return Texture::Create(m_device.Get(), m_wicFactory.Get(), filename);
    }
  
    void Graphics::DestroyTexture(wak::Texture* texture)
    {
        Texture::Destroy(texture);
    }

    ID3D11Device* Graphics::GetDevice() const
    {
        return m_device.Get();
    }
}
