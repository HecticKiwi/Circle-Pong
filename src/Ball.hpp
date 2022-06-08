#pragma once

#include "raylib.h"

class Ball
{
public:
    Ball() {}
    Ball(Texture* texture);
    ~Ball() {}

    enum State {
        ALIVE,
        DYING,
        DEAD,
    };

    void update();
    void draw();

    void reset();
    void newAngle();
    void explode();

    Vector2 getPos();
    State getState();

private:
    Vector2 pos;
    float angle;
    float speed;
    float w, h;
    float scale;
    State state;
    float alpha;

    Texture* texture;
};