#pragma once

#include <DirectXMath.h>

#include "WinArcadeKit/Vertex.h"

namespace wak
{
    class Graphics;
    class Texture;

    
    // Verts are pre-transformed on the CPU, so the framework draws with an identity
    // model matrix while view/projection (the camera) come from Graphics' state.
    class SpriteBatch
    {
    public:
        explicit SpriteBatch(Graphics& graphics);

        SpriteBatch(const SpriteBatch&)            = delete;
        SpriteBatch& operator=(const SpriteBatch&) = delete;

        void Begin();
        void Draw(Texture* texture,
                  DirectX::XMFLOAT2 position,
                  DirectX::XMFLOAT2 scale    = { 1.0f, 1.0f },
                  float             rotation = 0.0f,
                  DirectX::XMFLOAT4 tint     = { 1.0f, 1.0f, 1.0f, 1.0f });
        void End();

    private:
        void Flush();

        // TOOD: I don't knoow need to do something about this.
        static constexpr unsigned int MAX_QUADS      = 512;
        static constexpr unsigned int VERTS_PER_QUAD = 6;
        static constexpr unsigned int MAX_VERTICES   = MAX_QUADS * VERTS_PER_QUAD;

        Graphics&      m_gfx;
        TexturedVertex m_vertices[MAX_VERTICES];
        unsigned int   m_quadCount      = 0;
        Texture*       m_currentTexture = nullptr;
    };
}
