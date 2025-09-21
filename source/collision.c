#include <raylib.h>
#include "pong.h"

void CheckCollisions(Game *game)
{
    Env *env = game->env;
    Ball *ball = game->ball;

    if (ball->position.x <= env->screenWidth/2)
    {
        CheckCollisionPlayerBall(game);
    }
    else
    {
        CheckCollisionBotBall(game);
    }
}

void CheckCollisionPlayerBall(Game *game)
{
    Env *env = game->env;
    Player *player = game->player;
    Ball *ball = game->ball;
    Sounds *sounds = env->sounds;

    if(CheckCollisionCircleLine(
        ball->position,
        ball->radius,
        (Vector2){
            player->position.x+player->size.x,
            player->position.y
        },
        (Vector2){
            player->position.x+player->size.x,
            player->position.y+player->size.y
        }
    ))
    {
        PlaySound(sounds->paddle);
        ball->speed.x *= -1;
        ball->speed.y = COEF* \
        (ball->position.y-player->position.y-player->size.y/2)/player->size.y;
    }
}

void CheckCollisionBotBall(Game *game)
{
    Env *env = game->env;
    Bot *bot = game->bot;
    Ball *ball = game->ball;
    Sounds *sounds = env->sounds;

    if(CheckCollisionCircleLine(
        ball->position,
        ball->radius,
        (Vector2){bot->position.x, bot->position.y},
        (Vector2){bot->position.x, bot->position.y+bot->size.y}
    ))
    {
        PlaySound(sounds->paddle);
        ball->speed.x *= -1;
        ball->speed.y = COEF* \
        (ball->position.y-bot->position.y-bot->size.y/2)/bot->size.y;
    }
}
