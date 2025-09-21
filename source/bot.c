#include <raylib.h>
#include <math.h>
#include "pong.h"

void InitBot(Game *game)
{
    Env *env = game->env;
    Bot *bot = game->bot;

    bot->size = (Vector2){env->screenWidth/100, env->screenHeight/10};
    bot->speed = PADDLE_SPEED;
    ResetBot(game);
}

void UpdateBot(Game *game)
{
    Env *env = game->env;
    Bot *bot = game->bot;
    Ball *ball = game->ball;
    static bool centerFlag = false;

    if (ball->position.x > env->screenWidth/2 && ball->speed.x > 0)
    {
        if (!centerFlag && bot->position.y + bot->size.y/2 > ball->position.y)
        {
            bot->position.y -= bot->speed * env->deltaTime;
            if (bot->position.y < 0)
                bot->position.y = 0;
        }
        if (!centerFlag && bot->position.y + bot->size.y/2 <= ball->position.y)
        {
            bot->position.y += bot->speed * env->deltaTime;
            if (bot->position.y + bot->size.y > env->screenHeight)
                bot->position.y = env->screenHeight - bot->size.y;
        }
    
        if(fabsf(bot->position.y + bot->size.y/2 - ball->position.y) < 5.0f)
            centerFlag = true;
        if(fabsf(bot->position.y + bot->size.y/2 - ball->position.y) \
            >= bot->size.y/2)
            centerFlag = false;
    }
}

void ResetBot(Game *game)
{
    Env *env = game->env;
    Bot *bot = game->bot;

    bot->position = (Vector2){
        env->screenWidth - 2*bot->size.x,
        env->screenHeight/2 - bot->size.y/2
    };
}

void DrawBot(Game *game)
{
    Bot *bot = game->bot; 
    
    DrawRectangleRounded(
        (Rectangle)
        {
            bot->position.x,
            bot->position.y,
            bot->size.x,
            bot->size.y,
        },
        1,
        1,
        WHITE
    );
}
