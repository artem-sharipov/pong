#ifndef PONG_H
#define PONG_H

#include <raylib.h>

#define TARGET_FPS (60)

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

#endif // PONG_H
