#include "WinArcadeKit/SpriteBatch.h"

#include "WinArcadeKit/Graphics.h"
#include "WinArcadeKit/Texture.h"

using namespace DirectX;

namespace wak
{
    SpriteBatch::SpriteBatch(Graphics* graphics)
        : m_gfx(graphics)
    {
    }

    void SpriteBatch::Begin()
    {
        m_quadCount      = 0;
        m_currentTexture = nullptr;

        m_gfx->SetModelMatrix(XMMatrixIdentity());
    }

    void SpriteBatch::End()
    {
        Flush();
    }

    void SpriteBatch::Draw(Texture* texture, XMFLOAT2 position, XMFLOAT2 scale, float rotation, XMFLOAT4 tint)
    {
        if (!texture) return;

        // Flush on texture switch or capacity. Bare-bones = one texture per batch.
        const bool textureChanged = (m_currentTexture != nullptr) && (m_currentTexture != texture);
        const bool full           = (m_quadCount >= MAX_QUADS);
        if (textureChanged || full)
            Flush();

        m_currentTexture = texture;

        const float halfW = static_cast<float>(texture->GetWidth())  * 0.5f;
        const float halfH = static_cast<float>(texture->GetHeight()) * 0.5f;

        const XMMATRIX transform = XMMatrixScaling(scale.x, scale.y, 1.0f)
                                 * XMMatrixRotationZ(rotation)
                                 * XMMatrixTranslation(position.x, position.y, 0.0f);

        const XMFLOAT2 corners[4] = {
            { -halfW, -halfH }, 
            {  halfW, -halfH }, 
            { -halfW,  halfH }, 
            {  halfW,  halfH }, 
        };
        const XMFLOAT2 uvs[4] = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 0.0f, 1.0f },
            { 1.0f, 1.0f },
        };

        XMFLOAT2 transformed[4];
        for (int i = 0; i < 4; ++i)
            XMStoreFloat2(&transformed[i], XMVector2Transform(XMLoadFloat2(&corners[i]), transform));

        const auto r = static_cast<unsigned char>(tint.x * 255.0f);
        const auto g = static_cast<unsigned char>(tint.y * 255.0f);
        const auto b = static_cast<unsigned char>(tint.z * 255.0f);
        const auto a = static_cast<unsigned char>(tint.w * 255.0f);

        static constexpr int quadIdx[VERTS_PER_QUAD] = { 0, 2, 1, 1, 2, 3 };

        TexturedVertex* v = &m_vertices[m_quadCount * VERTS_PER_QUAD];
        for (int i = 0; i < static_cast<int>(VERTS_PER_QUAD); ++i)
        {
            const int idx = quadIdx[i];
            v[i] = TexturedVertex(transformed[idx].x, transformed[idx].y,
                                  uvs[idx].x,         uvs[idx].y,
                                  r, g, b, a);
        }

        ++m_quadCount;
    }

    void SpriteBatch::Flush()
    {
        if (m_quadCount == 0 || !m_currentTexture)
            return;

        m_gfx->DrawTextured(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
                           m_vertices,
                           m_quadCount * VERTS_PER_QUAD,
                           m_currentTexture);

        m_quadCount = 0;
    }
}
