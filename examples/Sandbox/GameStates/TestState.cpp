#include "TestState.h"
#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "WinArcadeKit/Graphics.h"
#include "WinArcadeKit/Vertex.h"

#include <Windows.h>
#ifdef _DEBUG
#include <iostream>
#endif

void TestState::OnUpdate(wak::Application& app, float dt)
{
	wak::Keyboard* keyboard = app.GetKeyboard();
	wak::Graphics& graphics = app.GetGraphics();

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

	graphics.ClearBuffer(0.39f, 0.58f, 0.93f, 1.0f); // Cornflower blue
	graphics.DrawTestTriangle(x, y, m_angle);
	m_angle += 0.016f; // TODO: use dt instead of hardcoding the angle increment.
    graphics.EndFrame();
}

void TestState::OnRender(wak::Application& app)
{
    const wak::Vertex vertices[] = {
        { 0.0f, 0.5f, 255, 0, 0, 0 },
        { 0.5f, -0.5f, 0, 255, 0, 0  },
        { -0.5f, -0.5f, 0, 0, 255, 0  },
        { -0.3f, 0.3f, 255, 0, 0, 0  },
        { 0.3f, 0.3f, 0, 255, 0, 0  },
        { 0.0f, -1.f, 0, 0, 255, 0  },
    };

    //const ConstantBuffer constantBufferData = {
    //    {
    //        XMMatrixTranspose(
    //            XMMatrixRotationZ(angle) *
    //            XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) * // TODO: aspect ratio will be adjusted for when we use orthographic projection.
    //            XMMatrixTranslation(x, y, 0.0f)
    //        )
    //    }
    //};
}
