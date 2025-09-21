#include <math.h>
#include "pong.h"
#include "version.h"

// Font sizes.
#define FONTSIZE_SMALL (20) 
#define FONTSIZE_NORMAL (30)
#define FONTSIZE_BIG (50)
#define FONTSIZE_LOGO (160)

void InitGame(Game *game)
{
    InitSounds(game);
    InitPlayer(game);
    InitBot(game);
    InitBall(game);
}

void UpdateDrawFrame(Game *game)
{
    UpdateGame(game);
    BeginDrawing();
    DrawGame(game);
    EndDrawing();
}

void DeinitGame(Game *game)
{
    DeinitSounds(game);
}

void UpdateGame(Game *game)
{
    Env *env = game->env;
    env->deltaTime = GetFrameTime();
    switch (env->currentScreen)
    {
        case LOGO:
            UpdateLogoScreen(game);
            break;

        case TITLE:
            UpdateTitleScreen(game);
            break;

        case GAMEPLAY:
            UpdateGameplayScreen(game);
    }
}

void UpdateLogoScreen(Game *game)
{
    Env *env = game->env;
    if (GetTime() > 2.0)
    {
        env->currentScreen = TITLE;
    }
}

void UpdateTitleScreen(Game *game)
{
    Env *env = game->env;
    Sounds *sounds = env->sounds;

    if (IsKeyPressed(KEY_SPACE))
    {
        env->currentScreen = GAMEPLAY;
        PlaySound(sounds->start);
    }
}

void UpdateGameplayScreen(Game *game)
{
    Env *env = game->env;
    if (IsKeyPressed('P')) env->pause = !env->pause;
    if (IsKeyPressed('R')) ResetGame(game);

    if (!env->pause)
    {
        UpdatePlayer(game);
        UpdateBot(game);
        UpdateBall(game);
        CheckCollisions(game);
    }
}

void ResetGame(Game *game)
{
    Env *env = game->env;
    Sounds *sounds = env->sounds;

    env->leftSideScore = 0;
    env->rightSideScore = 0;

    PlaySound(sounds->start);
    ResetPlayer(game);
    ResetBot(game);
    ResetBall(game);
}

void DrawGame(Game *game)
{
    Env *env = game->env;
    ClearBackground(BLACK);
    // DrawFPS(10, 10);
    // DrawText(TextFormat("DeltaTime: %.4f", GetFrameTime()), 10, 40, 20, GREEN);
    // DrawText(TextFormat("Ball pos: %.1f, %.1f", game->ball->position.x, game->ball->position.y), 10, 70, 20, GREEN);
    switch (env->currentScreen)
    {
        case LOGO:
            DrawLogoScreen(game);
            break;

        case TITLE:
            DrawTitleScreen(game);
            break;

        case GAMEPLAY:
            DrawGameplayScreen(game);
    }
}

void DrawLogoScreen(Game *game)
{
    Env *env = game->env;
    char *logoText = "PONG";

    DrawRectangle(0, 0, env->screenWidth, env->screenHeight, BLACK);
    DrawText(
        logoText,
        env->screenWidth/2 - MeasureText(logoText, FONTSIZE_LOGO)/2,
        env->screenHeight/2 - FONTSIZE_LOGO/2,
        FONTSIZE_LOGO,
        LIGHTGRAY
    );
}

void DrawTitleScreen(Game *game)
{
    Env *env = game->env;
    char *welcomeText = "Welcome to the Pong game!";
    char *controlsText_1 = "Use UP and DOWN keys for control paddle.";
    char *controlsText_2 = "Use key P for pause.";
    char *controlsText_3 = "Use key R for reset score.";
    char *tipText = "Press key SPACE to start the game.";
    char *versionText = VERSION_STRING;

    DrawRectangle(0, 0, env->screenWidth, env->screenHeight, DARKGRAY);
    DrawText(
        welcomeText,
        env->screenWidth/2 - MeasureText(welcomeText, FONTSIZE_NORMAL)/2,
        env->screenHeight/10,
        FONTSIZE_NORMAL,
        LIGHTGRAY
    );
    DrawText(
        controlsText_1,
        env->screenWidth/2 - MeasureText(controlsText_1, FONTSIZE_NORMAL)/2,
        env->screenHeight/10 + 2*FONTSIZE_NORMAL,
        FONTSIZE_NORMAL,
        LIGHTGRAY
    );
    DrawText(
        controlsText_2,
        env->screenWidth/2 - MeasureText(controlsText_2, FONTSIZE_NORMAL)/2,
        env->screenHeight/10 + 3*FONTSIZE_NORMAL,
        FONTSIZE_NORMAL,
        LIGHTGRAY
    );
    DrawText(
        controlsText_3,
        env->screenWidth/2 - MeasureText(controlsText_3, FONTSIZE_NORMAL)/2,
        env->screenHeight/10 + 4*FONTSIZE_NORMAL,
        FONTSIZE_NORMAL,
        LIGHTGRAY
    );
    DrawText(
        tipText,
        env->screenWidth/2 - MeasureText(tipText, FONTSIZE_NORMAL)/2,
        env->screenHeight/10 + 7*FONTSIZE_NORMAL,
        FONTSIZE_NORMAL,
        LIGHTGRAY
    );
    DrawText(
        versionText,
        10,
        env->screenHeight-FONTSIZE_SMALL,
        FONTSIZE_SMALL,
        LIGHTGRAY
    );
}

void DrawGameplayScreen(Game *game)
{
    DrawEnv(game);
    DrawPlayer(game);
    DrawBot(game);
    DrawBall(game);
    DrawPause(game);
}

void DrawEnv(Game *game)
{
    Env *env = game->env; 
    
    DrawRectangleLines(
        0,
        0,
        env->screenWidth,
        env->screenHeight,
        WHITE
    );
    DrawLineV(
        (Vector2){env->screenWidth/2, 0},
        (Vector2){env->screenWidth/2, env->screenHeight},
        WHITE
    );
    DrawText(
        TextFormat("%i",env->leftSideScore),
        env->screenWidth/4,
        env->screenHeight/30,
        FONTSIZE_BIG,
        WHITE
    );
    DrawText(
        TextFormat("%i", env->rightSideScore),
        3*env->screenWidth/4,
        env->screenHeight/30,
        FONTSIZE_BIG,
        WHITE
    );
}

void DrawPause(Game *game)
{
    Env *env = game->env;
    char *pauseText = "PAUSE";
    char *versionText = VERSION_STRING;

    if (env->pause)
    {
        DrawRectangle(
            0,
            0,
            env->screenWidth,
            env->screenHeight,
            (Color){80, 80, 80, 128}
        );

        DrawRectangleRounded(
            (Rectangle)
            {
                env->screenWidth/2-MeasureText(pauseText, FONTSIZE_NORMAL)/ \
                2-10,
                env->screenHeight/30,
                MeasureText(pauseText, FONTSIZE_NORMAL)+20,
                FONTSIZE_NORMAL+15,
            },
            0.8,
            1,
            DARKGRAY
        );

        DrawText(
            "PAUSE",
            env->screenWidth/2 - MeasureText(pauseText, FONTSIZE_NORMAL)/2,
            env->screenHeight/30+10,
            FONTSIZE_NORMAL,
            LIGHTGRAY
        );
        DrawText(
            versionText,
            10,
            env->screenHeight-FONTSIZE_SMALL,
            FONTSIZE_SMALL,
            LIGHTGRAY
        );
    }
}
