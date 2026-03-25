#include "MainMenu.h"
#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"
#include "WinArcadeKit/Graphics.h"

#include <Windows.h>
#ifdef _DEBUG
#include <iostream>
#endif

void MainMenu::OnUpdate(wak::Application& app, float dt)
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

    graphics.ClearBuffer(1.0f, 1.0f, 0.0f, 1.0f);
    graphics.EndFrame();
}