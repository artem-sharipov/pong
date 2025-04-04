#include <stdio.h>
#include "pong.h"

int main(int argc, char *argv[])
{
    // Command line arguments not used.
    (void)argc;
    (void)argv;

    // Initialize game environment.
    Sounds sounds = {0};
    Env env = {
        .title="Pong",
        .screenWidth=1280,
        .screenHeight=800,
        .sounds = &sounds
    };
    Player player = {0}; // Define player paddle.
    Bot bot = {0};       // Define bot paddle.
    Ball ball = {0};     // Define ball.

    // Initialize main game structure.
    Game game = {
        .env = &env,
        .player = &player,
        .bot = &bot,
        .ball = &ball
    };

    InitWindow(env.screenWidth, env.screenHeight, env.title);
    Image icon = LoadImage("../resources/images/ico.png");
    SetWindowIcon(icon);
    InitAudioDevice();
    InitGame(&game);
    SetTargetFPS(TARGET_FPS);
    while(WindowShouldClose() == false) 
    {
        UpdateDrawFrame(&game);
    }

    DeinitGame(&game);
    CloseAudioDevice();
    CloseWindow();
}