#include "ImmediateMode.h"

using Microsoft::WRL::ComPtr;

namespace wak
{
    ImmediateMode* ImmediateMode::CreateImmediateMode(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext)
    {
        /***********Create Dynamic Vertex Buffer************/
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

        return nullptr;
    }

    void ImmediateMode::DestroyImmediateMode(ImmediateMode* device)
    {

    }
}