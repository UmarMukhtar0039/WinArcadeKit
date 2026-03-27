#include "WinArcadeKit/Graphics.h"

#include <d3dcompiler.h>
#include <memory>

using Microsoft::WRL::ComPtr;

namespace wak
{
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

    void Graphics::DrawTestTriangle()
    {
        struct Vertex
        {
            struct {
                float x, y;
            }pos;

            struct {
			    unsigned char r, g, b, a;
            }color;
		};

        const Vertex vertices[] = {
            { 0.0f, 0.5f, 255, 0, 0, 0 },
            { 0.5f, -0.5f, 0, 255, 0, 0  },
            { -0.5f, -0.5f, 0, 0, 255, 0  },
            { -0.3f, 0.3f, 255, 0, 0, 0  },
            { 0.3f, 0.3f, 0, 255, 0, 0  },
            { 0.0f, -0.8f, 0, 0, 255, 0  },
		};

		// vertex buffer
		D3D11_BUFFER_DESC vBufferDesc = {};
		vBufferDesc.ByteWidth = sizeof(vertices);
		vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0u;
		vBufferDesc.MiscFlags = 0u;
		vBufferDesc.StructureByteStride = sizeof(Vertex);
		
        D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;

		ComPtr<ID3D11Buffer> vertexBuffer;
        HRESULT buff = m_device->CreateBuffer(
            &vBufferDesc,
            &initData,
			&vertexBuffer
		);

        const UINT stride = sizeof(Vertex);
        const UINT offset = 0u;
        m_deviceContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

		const unsigned short indices[] = { 
            0, 1, 2, 
            0, 2, 3,
            0, 4, 1,
			2, 1, 5,
        };
        
		// index buffer
        D3D11_BUFFER_DESC iBufferDesc = {};
        iBufferDesc.ByteWidth = sizeof(indices);
        iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        iBufferDesc.CPUAccessFlags = 0u;
        iBufferDesc.MiscFlags = 0u;
        iBufferDesc.StructureByteStride = sizeof(unsigned short);

        D3D11_SUBRESOURCE_DATA iInitData = {};
        iInitData.pSysMem = indices;

        ComPtr<ID3D11Buffer> indexBuffer;
        HRESULT iBuff = m_device->CreateBuffer(
            &iBufferDesc,
            &iInitData,
            &indexBuffer
        );

		m_deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

        // shaders setup
		ComPtr<ID3DBlob> shaderBlob;

        // create pixel shader
		ComPtr<ID3D11PixelShader> pixelShader;
		D3DReadFileToBlob(L"PixelShader.cso", &shaderBlob);
        m_device->CreatePixelShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            &pixelShader);

		// bind pixel shader to pipeline
		m_deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0u);

        // crate vshader
		ComPtr<ID3D11VertexShader> vertexShader;
        D3DReadFileToBlob(L"VertexShader.cso", &shaderBlob);
        m_device->CreateVertexShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            &vertexShader
		);

		// bind vertex shader to pipeline
		m_deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

        // input layout
        ComPtr<ID3D11InputLayout> inputLayout;
        const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        m_device->CreateInputLayout(
            inputElementDesc,
            (UINT)std::size(inputElementDesc),
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            &inputLayout);

		m_deviceContext->IASetInputLayout(inputLayout.Get());

		m_deviceContext->OMSetRenderTargets(1u, m_renderTarget.GetAddressOf(), NULL);

		D3D11_VIEWPORT viewport = {};
		viewport.Width = 1280.0f;
		viewport.Height = 720.0f;
		viewport.MaxDepth = 1.0f;

		m_deviceContext->RSSetViewports(1u, &viewport); // rasterizer stage viewport setup

		m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_deviceContext->DrawIndexed( (UINT)std::size(indices), 0u, 0u);
    }
}
