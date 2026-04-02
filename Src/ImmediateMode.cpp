#include "ImmediateMode.h"
#include "WinArcadeKit/Vertex.h"

#include "VertexShader.h"
#include "PixelShader.h"

#include <utility>
#include <iterator>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace wak
{
    ImmediateMode* ImmediateMode::Create(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext)
    {
        /***********Dynamic Vertex Buffer************/
        D3D11_BUFFER_DESC vBufferDesc = {};
        vBufferDesc.ByteWidth = sizeof(Vertex) * MAX_VERTICES;
        vBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// NOTE: Subresource data is not needed for dynamic buffers. We will update the buffer with Map/Unmap.
        ComPtr<ID3D11Buffer> vertexBuffer;
        HRESULT hr = d3dDevice->CreateBuffer(
            &vBufferDesc,
            nullptr,
            &vertexBuffer
        );

        if (FAILED(hr))
            return nullptr;
        /***********Dynamic Vertex Buffer************/

        /***********Dynamic Constant Buffer************/
        D3D11_BUFFER_DESC cBufferDesc = {};
        cBufferDesc.ByteWidth = sizeof(XMMATRIX);
        cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        ComPtr<ID3D11Buffer> constantBuffer;
        hr = d3dDevice->CreateBuffer(
            &cBufferDesc,
            nullptr,
            &constantBuffer
        );

        if (FAILED(hr))
            return nullptr;
        /***********Dynamic Constant Buffer************/

        // create pixel shader
        ComPtr<ID3D11PixelShader> pixelShader;
        d3dDevice->CreatePixelShader(g_PixelShaderBytecode, sizeof(g_PixelShaderBytecode), nullptr, &pixelShader);
        d3dDeviceContext->PSSetShader(pixelShader.Get(), nullptr, 0u);

        // create vertex shader
        ComPtr<ID3D11VertexShader> vertexShader;
        d3dDevice->CreateVertexShader(g_VertexShaderBytecode, sizeof(g_VertexShaderBytecode), nullptr, &vertexShader);
        d3dDeviceContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

        // input layout
        const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        ComPtr<ID3D11InputLayout> inputLayout;
        hr = d3dDevice->CreateInputLayout(
                inputElementDesc,
                (UINT)std::size(inputElementDesc),
                g_VertexShaderBytecode,
                sizeof(g_VertexShaderBytecode),
                &inputLayout);

        if (FAILED(hr))
            return nullptr;

        ImmediateMode* im = new ImmediateMode();
        im->m_deviceContext = d3dDeviceContext;
        im->m_vertexShader = std::move(vertexShader);
        im->m_pixelShader = std::move(pixelShader);
        im->m_inputLayout = std::move(inputLayout);
        im->m_vertexBuffer = std::move(vertexBuffer);
        im->m_constantBuffer = std::move(constantBuffer);
        XMStoreFloat4x4(&im->m_modelMatrix, XMMatrixIdentity());
        return im;
    }

    void ImmediateMode::Destroy(ImmediateMode* mode)
    {
        delete mode;
    }

    void ImmediateMode::SetModelMatrix(XMMATRIX modelMatrix)
    {
        XMStoreFloat4x4(&m_modelMatrix, modelMatrix);
    }

    void ImmediateMode::Draw(D3D11_PRIMITIVE_TOPOLOGY topology, const Vertex* vertices, unsigned int vertexCount)
    {
        if (vertexCount == 0 || vertexCount > MAX_VERTICES)
            return;

        // 1. upload vertices
        D3D11_MAPPED_SUBRESOURCE mapped = {};
        HRESULT hr = m_deviceContext->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        if (FAILED(hr)) return;
    
        memcpy(mapped.pData, vertices, vertexCount * sizeof(Vertex));
        m_deviceContext->Unmap(m_vertexBuffer.Get(), 0);
    
        // 2. update constant buffer with transposed model matrix
        hr = m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        if (FAILED(hr)) return;

        XMMATRIX transposed = XMMatrixTranspose(XMLoadFloat4x4(&m_modelMatrix));
        memcpy(mapped.pData, &transposed, sizeof(XMMATRIX));
        m_deviceContext->Unmap(m_constantBuffer.Get(), 0);

        // 3. bind pipeline state
        const UINT stride = sizeof(Vertex);
        const UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
        m_deviceContext->IASetInputLayout(m_inputLayout.Get());
        m_deviceContext->IASetPrimitiveTopology(topology);

        m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
        m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

        m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

        // 4. draw
        m_deviceContext->Draw(vertexCount, 0);
    }
}