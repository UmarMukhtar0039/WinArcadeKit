#pragma once

namespace wak
{
    struct Vertex
    {
        struct {
            float x, y;
        }pos;

        struct {
            unsigned char r, g, b, a;
        }color;

        Vertex() = default;

        Vertex(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        {
            pos.x = x;
            pos.y = y;
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }
    };

    struct TexturedVertex
    {
        struct {
            float x, y;
        }pos;

        struct {
            float u, v;
        }uv;

        struct {
            unsigned char r, g, b, a;
        }color;

        TexturedVertex() = default;

        TexturedVertex(float x, float y, float u, float v, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        {
            pos.x = x;
            pos.y = y;
            uv.u = u;
            uv.v = v;
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }
    };
}