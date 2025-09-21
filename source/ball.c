#include <raylib.h>
#include "pong.h"

void InitBall(Game *game)
{
    Ball *ball = game->ball;

    ball->radius = BALL_RADIUS;
    ResetBall(game);
}

void UpdateBall(Game *game)
{
    Env *env = game->env;
    Ball *ball = game->ball;
    Sounds *sounds = env->sounds;

    ball->position.x += ball->speed.x * env->deltaTime;
    if (ball->position.x + ball->radius >= env->screenWidth)
    {
        ++env->leftSideScore;
        PlaySound(sounds->win);
        ResetBall(game);

    } 
    if (ball->position.x - ball->radius <= 0)
    {
        ++env->rightSideScore;
        PlaySound(sounds->lose);
        ResetBall(game);
    }

    ball->position.y += ball->speed.y * env->deltaTime;
    if (ball->position.y + ball->radius >= env->screenHeight)
    {
        ball->speed.y *= -1;
        PlaySound(sounds->border);
    }

    if (ball->position.y - ball->radius <= 0)
    {
        ball->speed.y *= -1;
        PlaySound(sounds->border);
    }
}

void ResetBall(Game *game)
{
    Env *env = game->env;
    Ball *ball = game->ball;

    ball->position.x = env->screenWidth/2;
    ball->position.y = env->screenHeight/2;
    const int ballDirections[2] = {-1, 1};
    ball->speed.x = BALL_SPEED*ballDirections[GetRandomValue(0, 1)];
    ball->speed.y = 0;
}

void DrawBall(Game *game)
{
    Ball *ball = game->ball; 
    
    DrawCircleV(
        ball->position,
        ball->radius,
        WHITE
    );
}
