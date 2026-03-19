#include "MainMenu.h"
#include "WinArcadeKit/Application.h"
#include "WinArcadeKit/Keyboard.h"

#include <Windows.h>
#ifdef _DEBUG
#include <iostream>
#endif

void MainMenu::OnUpdate(wak::Application& app, float dt)
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
}