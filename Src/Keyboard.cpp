#include "WinArcadeKit/Keyboard.h"
#include <Windows.h>

namespace wak
{
    Keyboard::Keyboard()
    {
        // initialize all keys to not pressed
        memset(m_keys, KEY_STATE_NONE, sizeof(m_keys));
    }

    Keyboard::~Keyboard()
    {
    }

    void Keyboard::Update()
    {
        BYTE rawState[KEY_COUNT] = {0};
		BOOL isSateRead = GetKeyboardState(rawState); // using Windows internal key state cache for polling key states.
        
        for(int i = 0; i < KEY_COUNT; i++)
        {
            bool isDown = (rawState[i] & 0x80) != 0; // Bit 7 or high bit is set if key is down
            bool wasDown = (m_keys[i] & KeyState::KEY_STATE_DOWN) != 0;

            unsigned char newState = 0;
            if (isDown)
            {
				newState |= KEY_STATE_DOWN;
            }
            if (isDown && !wasDown)
            {
                newState |= KEY_STATE_PRESSED;
            }
            if (!isDown && wasDown)
            {
                newState |= KEY_STATE_RELEASED;
            }

            m_keys[i] = newState;            
        }

    }

    bool Keyboard::IsKeyDown(int virtualKey) const
    {
        return  (m_keys[virtualKey] & KEY_STATE_DOWN) != 0;
    }

    bool Keyboard::IsKeyPressed(int virtualKey) const
    {
        return  (m_keys[virtualKey] & KEY_STATE_PRESSED) != 0;
    }

    bool Keyboard::IsKeyReleased(int virtualKey) const
    {
        return  (m_keys[virtualKey] & KEY_STATE_RELEASED) != 0;
    }

    unsigned char Keyboard::GetKeyState(int virtualKey) const
    {
        return m_keys[virtualKey];
    }
}