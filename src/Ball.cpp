#include "Ball.hpp"
#include "Globals.hpp"
#include "raymath.h"
#include <cmath>

inline constexpr float ACCELERATION = 0.1f;

Ball::Ball(Texture* texture)
{
    this->texture = texture;
    w = texture->width;
    h = texture->height;
    reset();
}

void Ball::update() {
    switch (state) {
        case ALIVE:
            pos.x += speed * std::cos(angle*DEG2RAD);
            pos.y += speed * std::sin(angle*DEG2RAD);
            break;
        case DYING:
            scale += 0.03f;
            alpha -= 0.03f;
            if (alpha < 0.0f)
                state = DEAD;
            break;
        case DEAD:
            break;
    }
}

void Ball::draw() {
    Rectangle srcRect = {0.0f, 0.0f, w, h};
    Rectangle destRect = {pos.x, pos.y, w*scale, h*scale};
    Vector2 origin = {w*scale/2.0f, h*scale/2.0f};

    DrawTexturePro(*texture, srcRect, destRect, origin, 0.0f, Fade(WHITE, alpha));
}

void Ball::reset() {
    pos = {globals::SCREEN_WIDTH/2.0f, globals::SCREEN_HEIGHT/2.0f};
    speed = 2.0f;
    state = ALIVE;
    scale = 1.0f;
    alpha = 1.0f;
    newAngle();
}

void Ball::newAngle() {
    Vector2 vectorToCenter = Vector2Subtract(globals::SCREEN_CENTER, pos);
    angle = std::atan2(vectorToCenter.y, vectorToCenter.x)*RAD2DEG + GetRandomValue(-40, 40);
    if (angle < 0)
        angle += 360;
    if (angle > 360)
        angle -= 360;
    speed += ACCELERATION;
}

Vector2 Ball::getPos() {
    return pos;
}

void Ball::explode() {
    state = DYING;
}

Ball::State Ball::getState() {
    return state;
}