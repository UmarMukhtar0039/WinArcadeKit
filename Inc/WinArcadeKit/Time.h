#pragma once

#include <cstdint>

namespace wak {

    struct Time
    {
        float deltaTime = 0.0f;            // Scaled dt (affected by timeScale)
        float unscaledDeltaTime = 0.0f;    // Raw dt (for UI/pause menus)
        float timeScale = 1.0f;            // Multiplier (1.0 = normal, 1.0 < x < 0.0 = slow-mo effect, 0.0 = paused)
        double timeElapsed = 0.0;          // Total unscaled time since start (double to avoid float overflow)
        uint64_t frameCount = 0;           // Total frames since start
    };
}
