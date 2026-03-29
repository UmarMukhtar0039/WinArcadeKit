#include "TestState.h"
#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "WinArcadeKit/Graphics.h"

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

    graphics.ClearBuffer(0.0f, 0.0f, 0.0f, 1.0f);
	graphics.DrawTestTriangle(x, y, m_angle);
	m_angle += 0.016f; // TODO: use dt instead of hardcoding the angle increment.
    graphics.EndFrame();
}