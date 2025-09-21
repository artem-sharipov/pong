#include <raylib.h>
#include "pong.h"

void InitSounds(Game *game)
{
    Env *env = game->env;
    Sounds *sounds = env->sounds;

    sounds->start = LoadSound("../resources/sounds/start.wav");
    sounds->win = LoadSound("../resources/sounds/win.wav");
    sounds->lose = LoadSound("../resources/sounds/lose.wav");
    sounds->paddle = LoadSound("../resources/sounds/paddle.wav");
    sounds->border = LoadSound("../resources/sounds/border.wav");
}

void DeinitSounds(Game *game)
{
    Env* env = game->env;
    Sounds *sounds = env->sounds;

    UnloadSound(sounds->start);
    UnloadSound(sounds->win);
    UnloadSound(sounds->lose);
    UnloadSound(sounds->paddle);
    UnloadSound(sounds->border);
}