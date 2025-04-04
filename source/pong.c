#include <math.h>
#include "pong.h"
#include "version.h"

// Game object constants
#define PADDLE_SPEED (8)
#define BALL_RADIUS (6)
#define BALL_SPEED (8)
#define COEF (17)

// Font sizes.
#define FONTSIZE_SMALL (20) 
#define FONTSIZE_NORMAL (30)
#define FONTSIZE_BIG (50)
#define FONTSIZE_LOGO (160)

// Init and deinit functions
static void InitSounds(Game *game);
static void InitPlayer(Game *game);
static void InitBot(Game *game);
static void InitBall(Game *game);
static void DeinitSounds(Game *game);

// Update functions
static void UpdateGame(Game *game);
static void UpdateLogoScreen(Game *game);
static void UpdateTitleScreen(Game *game);
static void UpdateGameplayScreen(Game *game);
static void UpdatePlayer(Game *game);
static void UpdateBot(Game *game);
static void UpdateBall(Game *game);
static void ResetGame(Game *game);
static void ResetPlayer(Game *game);
static void ResetBot(Game *game);
static void ResetBall(Game *game);

// Check collisions functions
static void CheckCollisions(Game *game);
static void CheckCollisionPlayerBall(Game *game);
static void CheckCollisionBotBall(Game *game);

// Draw functions
static void DrawGame(Game *game);
static void DrawLogoScreen(Game *game);
static void DrawTitleScreen(Game *game);
static void DrawGameplayScreen(Game *game);
static void DrawEnv(Game *game);
static void DrawPlayer(Game *game);
static void DrawBot(Game *game);
static void DrawBall(Game *game);
static void DrawPause(Game *game);

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

static void InitSounds(Game *game)
{
    Env *env = game->env;
    Sounds *sounds = env->sounds;

    sounds->start = LoadSound("../resources/sounds/start.wav");
    sounds->win = LoadSound("../resources/sounds/win.wav");
    sounds->lose = LoadSound("../resources/sounds/lose.wav");
    sounds->paddle = LoadSound("../resources/sounds/paddle.wav");
    sounds->border = LoadSound("../resources/sounds/border.wav");
}

static void InitPlayer(Game *game)
{
    Env *env = game->env;
    Player *player = game->player;
    
    player->size = (Vector2){env->screenWidth/100, env->screenHeight/10};
    player->speed = PADDLE_SPEED;
    ResetPlayer(game);
}

static void InitBot(Game *game)
{
    Env *env = game->env;
    Bot *bot = game->bot;

    bot->size = (Vector2){env->screenWidth/100, env->screenHeight/10};
    bot->speed = PADDLE_SPEED;
    ResetBot(game);
}

static void InitBall(Game *game)
{
    Ball *ball = game->ball;

    ball->radius = BALL_RADIUS;
    ResetBall(game);
}

static void DeinitSounds(Game *game)
{
    Env* env = game->env;
    Sounds *sounds = env->sounds;

    UnloadSound(sounds->start);
    UnloadSound(sounds->win);
    UnloadSound(sounds->lose);
    UnloadSound(sounds->paddle);
    UnloadSound(sounds->border);
}

static void UpdateGame(Game *game)
{
    Env *env = game->env;
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

static void UpdateLogoScreen(Game *game)
{
    Env *env = game->env;
    ++env->framesCounter;

    if (env->framesCounter > 2*TARGET_FPS)
    {
        env->currentScreen = TITLE;
        env->framesCounter = 0;
    }
}

static void UpdateTitleScreen(Game *game)
{
    Env *env = game->env;
    Sounds *sounds = env->sounds;

    if (IsKeyPressed(KEY_SPACE))
    {
        env->currentScreen = GAMEPLAY;
        PlaySound(sounds->start);
    }
}

static void UpdateGameplayScreen(Game *game)
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

static void UpdatePlayer(Game *game)
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
static void UpdateBot(Game *game)
{
    Env *env = game->env;
    Bot *bot = game->bot;
    Ball *ball = game->ball;
    static bool centerFlag = false;

    if (ball->position.x > env->screenWidth/2)
    {
        if (!centerFlag && bot->position.y + bot->size.y/2 > ball->position.y)
        {
            bot->position.y -= bot->speed;
            if (bot->position.y < 0)
                bot->position.y = 0;
        }
        if (!centerFlag && bot->position.y + bot->size.y/2 <= ball->position.y)
        {
            bot->position.y += bot->speed;
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

static void UpdateBall(Game *game)
{
    Env *env = game->env;
    Ball *ball = game->ball;
    Sounds *sounds = env->sounds;

    ball->position.x += ball->speed.x;
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

    ball->position.y += ball->speed.y;
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

static void ResetGame(Game *game)
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

static void ResetPlayer(Game *game)
{
    Env *env = game->env;
    Player *player = game->player;

    player->position = (Vector2){
        player->size.x,
        env->screenHeight/2 - player->size.y/2};
}

static void ResetBot(Game *game)
{
    Env *env = game->env;
    Bot *bot = game->bot;

    bot->position = (Vector2){
        env->screenWidth - 2*bot->size.x,
        env->screenHeight/2 - bot->size.y/2
    };
}

static void ResetBall(Game *game)
{
    Env *env = game->env;
    Ball *ball = game->ball;

    ball->position.x = env->screenWidth/2;
    ball->position.y = env->screenHeight/2;
    const int ballDirections[2] = {-1, 1};
    ball->speed.x = BALL_SPEED*ballDirections[GetRandomValue(0, 1)];
    ball->speed.y = 0;
}

static void CheckCollisions(Game *game)
{
    Env *env = game->env;
    //Player *player = game->player;
    //Bot *bot = game->bot;
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

static void CheckCollisionPlayerBall(Game *game)
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

static void CheckCollisionBotBall(Game *game)
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

static void DrawGame(Game *game)
{
    Env *env = game->env;
    ClearBackground(BLACK);
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

static void DrawLogoScreen(Game *game)
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

static void DrawTitleScreen(Game *game)
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

static void DrawGameplayScreen(Game *game)
{
    DrawEnv(game);
    DrawPlayer(game);
    DrawBot(game);
    DrawBall(game);
    DrawPause(game);
}

static void DrawEnv(Game *game)
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

static void DrawPlayer(Game *game)
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

static void DrawBot(Game *game)
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

static void DrawBall(Game *game)
{
    Ball *ball = game->ball; 
    
    DrawCircleV(
        ball->position,
        ball->radius,
        WHITE
    );
}

static void DrawPause(Game *game)
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
