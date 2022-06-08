#include "Paddle.hpp"
#include "raylib.h"
#include "Globals.hpp"
#include <cmath>

inline constexpr float ROTATION_SPEED = 3.0f;

Paddle::Paddle(Texture* texture) {
    this->texture = texture;
    w = texture->width;
    h = texture->height;
    rotation = 1.0f;
}

void Paddle::update() {
    if (IsKeyDown(KEY_LEFT))
        rotation -= ROTATION_SPEED;
    if (IsKeyDown(KEY_RIGHT))
        rotation += ROTATION_SPEED;
    if (rotation < 0)
        rotation += 360;
    if (rotation > 360)
        rotation -= 360;
}

void Paddle::draw() {
    float x = globals::SCREEN_WIDTH/2.0f + std::cos(rotation*DEG2RAD)*globals::RING_RADIUS;
    float y = globals::SCREEN_HEIGHT/2.0f + std::sin(rotation*DEG2RAD)*globals::RING_RADIUS;
    Rectangle srcRect = {0.0f, 0.0f, w, h};
    Rectangle destRect = {x, y, w, h};
    Vector2 origin = {w/2.0f, h/2.0f};

    DrawTexturePro(*texture, srcRect, destRect, origin, rotation, WHITE);
}

float Paddle::getRotation() {
    return rotation;
}