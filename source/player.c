#include <raylib.h>
#include "pong.h"

void InitPlayer(Game *game)
{
    Env *env = game->env;
    Player *player = game->player;
    
    player->size = (Vector2){env->screenWidth/100, env->screenHeight/10};
    player->speed = PADDLE_SPEED;
    ResetPlayer(game);
}

void UpdatePlayer(Game *game)
{
    Env *env = game->env;
    Player *player = game->player;

    if (IsKeyDown(KEY_UP))
    {
        player->position.y -= player->speed;
        if (player->position.y < 0) player->position.y = 0;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        player->position.y += player->speed;
        if ((player->position.y + player->size.y) > env->screenHeight)
            player->position.y = env->screenHeight - player->size.y;
    }

}

void ResetPlayer(Game *game)
{
    Env *env = game->env;
    Player *player = game->player;

    player->position = (Vector2){
        player->size.x,
        env->screenHeight/2 - player->size.y/2};
}

void DrawPlayer(Game *game)
{
    Player *player = game->player; 
    
    DrawRectangleRounded(
        (Rectangle)
        {
            player->position.x,
            player->position.y,
            player->size.x,
            player->size.y,
        },
        1,
        1,
        WHITE
    );
}