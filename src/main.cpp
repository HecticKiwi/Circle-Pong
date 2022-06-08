#include "raylib.h"
#include "raymath.h"
#include "Paddle.hpp"
#include "Globals.hpp"
#include "Ball.hpp"
#include <string>
#include <cmath>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void updateDrawFrame();
void update();
void draw();
void reset();

enum class State {
    MAIN_MENU,
    PLAYING,
    GAME_OVER,
};

// State and transition variables
static float transAlpha = 0;
static bool onTransition = false;
static bool transFadeOut = false;
static State transToScreen;
State currentScreen = State::MAIN_MENU;

void TransitionToScreen(State screen); // Request transition to next screen
void UpdateTransition(void);           // Update transition effect
void DrawTransition(void);             // Draw transition effect (full-screen rectangle)

Texture paddleTex;
Texture ringTex;
Texture ballTex;

Paddle paddle;
Ball ball;

Sound pointSFX;
Sound explosionSFX;

int score;

int main() 
{
    InitWindow(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, "Circle Pong");
    InitAudioDevice();
    pointSFX = LoadSound("res/sfx/point.wav");
    explosionSFX = LoadSound("res/sfx/explosion.wav");
    paddleTex = LoadTexture("res/gfx/paddle.png");
    ringTex = LoadTexture("res/gfx/ring.png");
    ballTex = LoadTexture("res/gfx/ball.png");
    SetTextureFilter(ballTex, TEXTURE_FILTER_POINT);
    reset();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(updateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())
        updateDrawFrame();
    
#endif
    UnloadTexture(paddleTex);
    UnloadTexture(ringTex);
    UnloadTexture(ballTex);
    UnloadSound(pointSFX);
    UnloadSound(explosionSFX);
    CloseWindow();
    CloseAudioDevice();
    return 0;
}

void updateDrawFrame() {
    update();
    draw();
}

void update() {
    if (onTransition) {
        UpdateTransition();
    } else {
        switch(currentScreen) {
            case State::MAIN_MENU:
                if (IsKeyPressed(KEY_SPACE)) {
                    TransitionToScreen(State::PLAYING);
                }
                break;
            case State::PLAYING: {
                Vector2 vectorToCenter = Vector2Subtract(globals::SCREEN_CENTER, ball.getPos());
                float angle = Vector2Angle(vectorToCenter, (Vector2){1.0f, 0.0f});
                bool ballLeavingCenter = Vector2Length(vectorToCenter) + globals::BALL_RADIUS/2.0f > globals::RING_RADIUS;
                bool ballTouchingPaddle = abs(paddle.getRotation() - angle) < globals::PADDLE_LENGTH || abs(paddle.getRotation() - angle) > 360 - globals::PADDLE_LENGTH;
                if (ballLeavingCenter && ball.getState() == Ball::ALIVE) {
                    if (ballTouchingPaddle) {
                        ball.newAngle();
                        score++;
                        PlaySound(pointSFX);
                    } else {
                        ball.explode();
                        PlaySound(explosionSFX);
                    }
                }

                paddle.update();
                ball.update();

                if (ball.getState() == ball.DEAD) {
                    TransitionToScreen(State::GAME_OVER);
                }
                break;
            }
            case State::GAME_OVER:
                if (IsKeyPressed(KEY_SPACE)) {
                    TransitionToScreen(State::PLAYING);
                    reset();
                }
                break;
        }
    }
}

void draw() {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    switch (currentScreen) {
        case State::MAIN_MENU:
            DrawText("Circle Pong", (globals::SCREEN_WIDTH - MeasureText("Circle Pong", 40))/2.0f, 100, 40, WHITE);
            DrawText("Press [space] to play", (globals::SCREEN_WIDTH - MeasureText("Press [space] to play", 30))/2.0f, 200, 30, LIGHTGRAY);
            break;
        case State::PLAYING: {
            DrawTexture(ringTex, globals::SCREEN_WIDTH/2.0f - 138, globals::SCREEN_HEIGHT/2.0f - 138, WHITE);
            std::string scoreText = std::to_string(score);
            int x = MeasureText(scoreText.c_str(), 40);
            DrawText(scoreText.c_str(), (globals::SCREEN_WIDTH - x)/2.0f, (globals::SCREEN_HEIGHT - 40)/2.0f, 40, LIGHTGRAY);

            paddle.draw();
            ball.draw();
            break;
        }
        case State::GAME_OVER:
            DrawText("Game over", (globals::SCREEN_WIDTH - MeasureText("Game over", 40))/2.0f, 100, 40, WHITE);
            DrawText((std::string("Score: ") + std::to_string(score)).c_str(), (globals::SCREEN_WIDTH - MeasureText((std::string("Score: ") + std::to_string(score)).c_str(), 30))/2.0f, 160, 30, LIGHTGRAY);
            DrawText("Press [space] to play again", (globals::SCREEN_WIDTH - MeasureText("Press [space] to play again", 30))/2.0f, 200, 30, LIGHTGRAY);
            break;
    }

    if (onTransition)
        DrawTransition();   
    
    // DrawFPS(10, 10);
    EndDrawing();
}

void reset() {
    paddle = Paddle(&paddleTex);
    ball = Ball(&ballTex);
    score = 0;
}

// Request transition to next screen
void TransitionToScreen(State newScreen) {
    onTransition = true;
    transToScreen = newScreen;
}

// Update transition effect
void UpdateTransition(void) {
    if (!transFadeOut)
    {
        transAlpha += 0.02f;

        if (transAlpha >= 1.0)
        {
            transAlpha = 1.0;
            currentScreen = transToScreen;
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha <= 0)
        {
            transAlpha = 0;
            transFadeOut = false;
            onTransition = false;
        }
    }
}

// Draw transition effect (full-screen rectangle)
void DrawTransition(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, transAlpha));
}
