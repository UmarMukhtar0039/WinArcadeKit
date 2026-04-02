#include "TestState.h"
#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "WinArcadeKit/Graphics.h"
#include "WinArcadeKit/Vertex.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <iterator>

#ifdef _DEBUG
#include <iostream>
#endif

using namespace DirectX;

void TestState::OnUpdate(wak::Application& app, float dt)
{
	wak::Keyboard* keyboard = app.GetKeyboard();

#ifdef _DEBUG
    if (keyboard->IsKeyPressed(VK_SPACE))
        std::cout << "SPACE pressed!" << std::endl;
    if (keyboard->IsKeyDown('W'))
        std::cout << "W down" << std::endl;
    if (keyboard->IsKeyReleased('W'))
        std::cout << "W released" << std::endl;
#endif


    if (keyboard->IsKeyDown('W'))
    {
		y += 0.01f;
    }
    if (keyboard->IsKeyDown('S'))
	{
        y -= 0.01f;
	}
	if (keyboard->IsKeyDown('A'))
    {
	    x -= 0.01f;
	}
    if (keyboard->IsKeyDown('D'))
    {
        x += 0.01f;
    }

	m_angle += 0.016f; // TODO: use dt instead of hardcoding the angle increment.
}

void TestState::OnRender(wak::Application& app)
{
	wak::Graphics& gfx = app.GetGraphics();

    const wak::Vertex vertices[] = {
        // triangle 0
        { 0.0f,  0.5f,  255, 0,   0,   255 },
        { 0.5f, -0.5f,  0,   255, 0,   255 },
        {-0.5f, -0.5f,  0,   0,   255, 255 },
        // triangle 1
        { 0.0f,  0.5f,  255, 0,   0,   255 },
        {-0.5f, -0.5f,  0,   0,   255, 255 },
        {-0.3f,  0.3f,  255, 0,   0,   255 },
        // triangle 2
        { 0.0f,  0.5f,  255, 0,   0,   255 },
        { 0.3f,  0.3f,  0,   255, 0,   255 },
        { 0.5f, -0.5f,  0,   255, 0,   255 },
        // triangle 3
        { 0.5f, -0.5f,  0,   255, 0,   255 },
        {-0.5f, -0.5f,  0,   0,   255, 255 },
        { 0.0f, -1.0f,  0,   0,   255, 255 },
    };

    XMMATRIX transform =
        XMMatrixRotationZ(m_angle) *
        XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) * // TODO: aspect ratio will be adjusted for when we use orthographic projection.
        XMMatrixTranslation(x, y, 0.0f);

	gfx.SetModelMatrix(transform);
    gfx.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vertices, std::size(vertices));
}
