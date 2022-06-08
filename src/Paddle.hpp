#pragma once

#include "raylib.h"

class Paddle
{
public:
    Paddle() {}
    Paddle(Texture* texture);
    ~Paddle() {}

    void update();
    void draw();

    float getRotation();
private:
    float rotation;
    float w, h;
    Texture* texture;
};