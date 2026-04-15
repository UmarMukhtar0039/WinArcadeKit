#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <wrl/client.h>

namespace wak
{
	struct Vertex;
	struct TexturedVertex;
	class ImmediateMode;
	class Texture;

    class Graphics
    {
    public:
        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;
    
		static Graphics* CreateDevice(HWND hwnd, FLOAT width, FLOAT height); // TODO: We'll return unique_ptr here once we decide on the ownership model for Graphics. For now, we'll just return a raw pointer and let the Application class manage its lifetime.
		static void DestroyDevice(Graphics* gfx); // TODO: This setup doesn't feel right. But it was necessary since we don't create the Graphics device via new so we shouldn't zdelete it either. Maybe we can use a unique_ptr.
    
        void EndFrame();
        void BeginFrame();
        void ClearBuffer(float r, float g, float b, float a);

        // Drawing api
        void Draw(D3D11_PRIMITIVE_TOPOLOGY topology, const Vertex* vertices, unsigned int count);
        void DrawTextured(D3D11_PRIMITIVE_TOPOLOGY topology, const TexturedVertex* vertices, unsigned int count, Texture* texture);
        void SetModelMatrix(DirectX::XMMATRIX transform);
        void SetViewMatrix(DirectX::XMMATRIX viewMatrix);
        void SetProjectionMatrix(DirectX::XMMATRIX projectionMatrix);
        void DestroyTexture(wak::Texture* texture);

        // Texture loading
        Texture* LoadTexture(const wchar_t* filename);
        ID3D11Device* GetDevice() const;
	    
    private:
        Graphics() = default;
        ~Graphics() = default; // NOTE: we are using ComPtr, so we don't need to manually release resources.
    
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;

        ImmediateMode* m_immediateMode = nullptr;
        Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicFactory;

        FLOAT m_width = 0.0f;
        FLOAT m_height = 0.0f;
    };

}