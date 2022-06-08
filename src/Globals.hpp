#pragma once

namespace globals {
    constexpr int SCREEN_WIDTH = 480;
    constexpr int SCREEN_HEIGHT = 360;

    inline const Vector2 SCREEN_CENTER = {globals::SCREEN_WIDTH/2.0f, globals::SCREEN_HEIGHT/2.0f};

    constexpr float RING_RADIUS = 123.0f;
    constexpr float BALL_RADIUS = 31.0f;
    constexpr float PADDLE_LENGTH = 35.0f;
}