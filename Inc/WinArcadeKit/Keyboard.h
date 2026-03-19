/*
* We are using polling of the Windows internal key state cache for simplicity and to avoid the overhead of processing Win32 messages for keyboard input. 
* There are a few problems with polling. We can't simulate the window lost focus scenario well without having some sort of band-aid code in place to do som.
* We'll need to handle WM_CHAR messages in future for texts.
* We'll resort to implementing event based system with a member windowProc Or using DirectXTK keyboard where they have event based inputs. 
*/

#pragma once

namespace wak
{
    class Keyboard
    {
        public:
            Keyboard();
            ~Keyboard();

            Keyboard(const Keyboard&) = delete;
            Keyboard& operator=(const Keyboard&) = delete;

            enum KeyState
            {
                KEY_STATE_NONE = 0,
                KEY_STATE_PRESSED = 1 << 0,
                KEY_STATE_DOWN = 1 << 1,
                KEY_STATE_RELEASED = 1 << 2,
            };

            void Update();

            bool IsKeyDown(int virtualKey) const;
            bool IsKeyPressed(int virtualKey) const;
            bool IsKeyReleased(int virtualKey) const;

            // imperitive, used when we lost focus on the window. See Application::Run
            void Reset() noexcept;
            

        private:
            // might not even use it ever.
            unsigned char GetKeyState(int virtualKey) const;

            // virtual key codes are in the range [0, 255]
            static constexpr int KEY_COUNT = 256;
            unsigned char m_keys[KEY_COUNT];
    };
}