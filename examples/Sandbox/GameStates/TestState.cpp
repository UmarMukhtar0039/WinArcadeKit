#include "TestState.h"
#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "WinArcadeKit/Graphics.h"
#include "WinArcadeKit/Vertex.h"
#include "WinArcadeKit/Texture.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <iterator>

#ifdef _DEBUG
#include <iostream>
#endif

using namespace DirectX;

void TestState::OnActivate(wak::Application& app, wak::StateArgs& args)
{
    m_testTexture = app.GetGraphics().LoadTexture(L"assets/characterSprite.png");
    m_testTextureBlending = app.GetGraphics().LoadTexture(L"assets/lamp.png");

#ifdef _DEBUG
    if (m_testTexture)
        std::cout << "Texture loaded: " << m_testTexture->GetWidth() << "x" << m_testTexture->GetHeight() << std::endl;
    else
        std::cout << "Failed to load texture!" << std::endl;
#endif
}

void TestState::OnDeactivate(wak::Application& app)
{
    app.GetGraphics().DestroyTexture(m_testTexture);
    m_testTexture = nullptr;
    
    app.GetGraphics().DestroyTexture(m_testTextureBlending);
    m_testTextureBlending = nullptr;
}

void TestState::OnUpdate(wak::Application& app, float deltaTime)
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

    const float MOVE_SPEED = 200.0f;   // world units per second
    if (keyboard->IsKeyDown('W'))
        y -= MOVE_SPEED * deltaTime;  // Y-down: up on screen = decrease Y
    if (keyboard->IsKeyDown('S'))
        y += MOVE_SPEED * deltaTime;
    if (keyboard->IsKeyDown('A'))
        x -= MOVE_SPEED * deltaTime;
    if (keyboard->IsKeyDown('D'))
        x += MOVE_SPEED * deltaTime;

    const float ROTATION_SPEED = 1.0f; // rads per second
	m_angle += ROTATION_SPEED * deltaTime;

	// FPS counter (smoothed)
	float unscaledDt = app.GetUnscaledDeltaTime();
	if (unscaledDt > 0.0f)
	{
		m_fpsSmooth = m_fpsSmooth * 0.95f + (1.0f / unscaledDt) * 0.05f;
#ifdef _DEBUG
		char fpsText[64];
		sprintf_s(fpsText, "%.0f FPS (%.2f ms)", m_fpsSmooth, unscaledDt * 1000.0f);
		//std::cout << fpsText << std::endl;
#endif
	}
}

void TestState::OnRender(wak::Application& app)
{
	wak::Graphics& gfx = app.GetGraphics();

    const wak::Vertex vertices[] = {
        // triangle 0
        {  0.0f,   25.0f,  255, 0,   0,   255 },
        {  25.0f, -25.0f,  0,   255, 0,   255 },
        { -25.0f, -25.0f,  0,   0,   255, 255 },
        // triangle 1
        {  0.0f,   25.0f,  255, 0,   0,   255 },
        { -25.0f, -25.0f,  0,   0,   255, 255 },
        { -15.0f,  15.0f,  255, 0,   0,   255 },
        // triangle 2
        {  0.0f,   25.0f,  255, 0,   0,   255 },
        {  15.0f,  15.0f,  0,   255, 0,   255 },
        {  25.0f, -25.0f,  0,   255, 0,   255 },
        // triangle 3
        {  25.0f, -25.0f,  0,   255, 0,   255 },
        { -25.0f, -25.0f,  0,   0,   255, 255 },
        {  0.0f,  -50.0f,  0,   0,   255, 255 },
    };

    XMMATRIX transform =
        XMMatrixRotationZ(m_angle) *
        XMMatrixTranslation(x, y, 0.0f);

	gfx.SetModelMatrix(transform);
    gfx.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vertices, std::size(vertices));

    // Textured quads
    gfx.DrawSprite(m_testTexture,         { 900.0f, 360.0f }, { 0.5f, 0.5f });
    gfx.DrawSprite(m_testTextureBlending, { 900.0f, 360.0f }, { 2.0f, 2.0f });
}

