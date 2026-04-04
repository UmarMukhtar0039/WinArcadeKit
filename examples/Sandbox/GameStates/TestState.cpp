#include "TestState.h"
#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "WinArcadeKit/Graphics.h"
#include "WinArcadeKit/Vertex.h"
#include "WinArcadeKit/Time.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <iterator>

#ifdef _DEBUG
#include <iostream>
#endif

using namespace DirectX;

void TestState::OnUpdate(wak::Application& app, const wak::Time& time)
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

    const float MOVE_SPEED = 0.6f;     // units per second
    if (keyboard->IsKeyDown('W'))
        y += MOVE_SPEED * time.deltaTime;
    if (keyboard->IsKeyDown('S'))
        y -= MOVE_SPEED * time.deltaTime;
    if (keyboard->IsKeyDown('A'))
        x -= MOVE_SPEED * time.deltaTime;
    if (keyboard->IsKeyDown('D'))
        x += MOVE_SPEED * time.deltaTime;

    const float ROTATION_SPEED = 1.0f; // rads per second
	m_angle += ROTATION_SPEED * time.deltaTime;
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
