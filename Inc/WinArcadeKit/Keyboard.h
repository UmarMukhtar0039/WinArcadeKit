#pragma once


namespace wak
{
    class Keyboard
    {
        public:
            Keyboard();
            ~Keyboard();

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

        private:
            Keyboard(const Keyboard&) = delete;
            Keyboard& operator=(const Keyboard&) = delete;

            // might not even use it ever.
            unsigned char GetKeyState(int virtualKey) const;

            // virtual key codes are in the range [0, 255]
            static constexpr int KEY_COUNT = 256;
            unsigned char m_keys[KEY_COUNT];
    };
}