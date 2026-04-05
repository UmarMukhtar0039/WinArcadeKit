#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

namespace wak
{
    struct Vertex;

    class ImmediateMode
    {
    public:
         ImmediateMode(const ImmediateMode&) = delete;
         ImmediateMode& operator=(const ImmediateMode&) = delete;


         static ImmediateMode* Create(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext, float width, float height);
		 static void Destroy(ImmediateMode* mode);

         void SetModelMatrix(DirectX::XMMATRIX modelMatrix);
         void SetViewMatrix(DirectX::XMMATRIX viewMatrix);
         void SetProjectionMatrix(DirectX::XMMATRIX projectionMatrix);
		 void Draw(D3D11_PRIMITIVE_TOPOLOGY topology, const Vertex* vertices, unsigned int vertexCount);

    private:
        ImmediateMode() = default;
		~ImmediateMode() = default;

        // TODO: auto-flush when full instead of silently discarding overflow and just dropping vertices on the floor.
        static const unsigned int MAX_VERTICES = 4096;

        // NOTE: Shouldn't be comPtr as it's owned by Graphics and we don't want to mess with reference counting here.
        ID3D11DeviceContext* m_deviceContext;

        Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_pixelShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_inputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer>        m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer>        m_constantBuffer;
        
        DirectX::XMFLOAT4X4 m_modelMatrix;
        DirectX::XMFLOAT4X4 m_viewMatrix;
        DirectX::XMFLOAT4X4 m_projectionMatrix;
    };
}