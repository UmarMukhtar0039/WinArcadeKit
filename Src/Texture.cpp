#include "WinArcadeKit/Texture.h"

#include <vector>

using Microsoft::WRL::ComPtr;

namespace wak
{
    Texture* Texture::Create(ID3D11Device* device, IWICImagingFactory* wicFactory, const wchar_t* filename)
    {
        // 1. Create decoder from file
        ComPtr<IWICBitmapDecoder> decoder;
        HRESULT hr = wicFactory->CreateDecoderFromFilename(
            filename,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &decoder
        );
        if (FAILED(hr)) return nullptr;

        // 2. Get first frame
        ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) return nullptr;

        // 3. Convert to RGBA 32bpp (matches DXGI_FORMAT_R8G8B8A8_UNORM)
        ComPtr<IWICFormatConverter> converter;
        hr = wicFactory->CreateFormatConverter(&converter);
        if (FAILED(hr)) return nullptr;

        hr = converter->Initialize(
            frame.Get(),
            GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0f,
            WICBitmapPaletteTypeMedianCut
        );
        if (FAILED(hr)) return nullptr;

        // 4. Get dimensions
        UINT width = 0, height = 0;
        hr = converter->GetSize(&width, &height);
        if (FAILED(hr)) return nullptr;

        // 5. Copy pixels into CPU buffer
        const UINT stride = width * 4; // 4 bytes per pixel (RGBA)
        const UINT bufferSize = stride * height;
        std::vector<BYTE> pixels(bufferSize);

        hr = converter->CopyPixels(nullptr, stride, bufferSize, pixels.data());
        if (FAILED(hr)) return nullptr;

        // 6. Create D3D11 texture
        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = pixels.data();
        initData.SysMemPitch = stride;

        ComPtr<ID3D11Texture2D> texture2D;
        hr = device->CreateTexture2D(&texDesc, &initData, &texture2D);
        if (FAILED(hr)) return nullptr;

        // 7. Create shader resource view
        ComPtr<ID3D11ShaderResourceView> srv;
        hr = device->CreateShaderResourceView(texture2D.Get(), nullptr, &srv);
        if (FAILED(hr)) return nullptr;

        // 8. Assemble Texture object
        Texture* texture = new Texture();
        texture->m_srv = std::move(srv);
        texture->m_width = width;
        texture->m_height = height;
        return texture;
    }

    void Texture::Destroy(Texture* texture)
    {
        delete texture;
    }

    unsigned int Texture::GetWidth() const { return m_width; }
    unsigned int Texture::GetHeight() const { return m_height; }
    ID3D11ShaderResourceView* Texture::GetSRV() const { return m_srv.Get(); }
}
