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
}