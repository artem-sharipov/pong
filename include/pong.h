#ifndef PONG_H
#define PONG_H

#include <raylib.h>

#define TARGET_FPS (60)

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

typedef struct Game Game;
typedef struct Env Env;
typedef struct Sounds Sounds;
typedef struct Player Player;
typedef struct Bot Bot;
typedef struct Ball Ball;

typedef enum GameScreen {
    LOGO = 0,
    TITLE,
    GAMEPLAY,
} GameScreen;

struct Game {
    Env* env;
    Player* player;
    Bot* bot;
    Ball* ball;
};

struct Env {
    const char *title;
    int screenWidth;
    int screenHeight;
    unsigned leftSideScore;
    unsigned rightSideScore;
    GameScreen currentScreen;
    unsigned framesCounter;
    bool pause;
    Sounds *sounds;
};

struct Sounds {
    Sound start;
    Sound win;
    Sound lose;
    Sound paddle;
    Sound border;
};

struct Player {
    Vector2 position;
    Vector2 size;
    int speed;
};

struct Bot {
    Vector2 position;
    Vector2 size;
    int speed;
};

struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
};

void InitGame(Game *game);
void UpdateDrawFrame(Game *game);
void DeinitGame(Game *game);

// Init and deinit functions
void InitSounds(Game *game);
void InitPlayer(Game *game);
void InitBot(Game *game);
void InitBall(Game *game);
void DeinitSounds(Game *game);

// Update functions
void UpdateGame(Game *game);
void UpdateLogoScreen(Game *game);
void UpdateTitleScreen(Game *game);
void UpdateGameplayScreen(Game *game);
void UpdatePlayer(Game *game);
void UpdateBot(Game *game);
void UpdateBall(Game *game);
void ResetGame(Game *game);
void ResetPlayer(Game *game);
void ResetBot(Game *game);
void ResetBall(Game *game);

// Check collisions functions
void CheckCollisions(Game *game);
void CheckCollisionPlayerBall(Game *game);
void CheckCollisionBotBall(Game *game);

// Draw functions
void DrawGame(Game *game);
void DrawLogoScreen(Game *game);
void DrawTitleScreen(Game *game);
void DrawGameplayScreen(Game *game);
void DrawEnv(Game *game);
void DrawPlayer(Game *game);
void DrawBot(Game *game);
void DrawBall(Game *game);
void DrawPause(Game *game);

#endif // PONG_H
