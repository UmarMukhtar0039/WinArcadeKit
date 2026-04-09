// TODO: This is just for learning purpose we'll switch to stb library or libpng in future.

#pragma once

#include <d3d11.h>
#include <wincodec.h>
#include <wrl/client.h>

namespace wak
{
    class Texture
    {
    public:
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        static Texture* Create(ID3D11Device* device, IWICImagingFactory* wicFactory, const wchar_t* filename);
        static void Destroy(Texture* texture);

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        ID3D11ShaderResourceView* GetSRV() const;

    private:
        Texture() = default;
        ~Texture() = default;

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
        unsigned int m_width = 0;
        unsigned int m_height = 0;
    };
}
