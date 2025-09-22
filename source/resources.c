#include <raylib.h>
#include "pong.h"
#include "embedded_resources.h"

static Image LoadEmbeddedIcon(void);
static Sound LoadBorderSound(void);
static Sound LoadLoseSound(void);
static Sound LoadPaddleSound(void);
static Sound LoadStartSound(void);
static Sound LoadWinSound(void);

void InitIco(void)
{
    Image icon = LoadEmbeddedIcon();
    SetWindowIcon(icon);
    UnloadImage(icon);
}

void InitSounds(Game *game)
{
    Env *env = game->env;
    Sounds *sounds = env->sounds;

    sounds->start = LoadStartSound();
    sounds->win = LoadStartSound();
    sounds->lose = LoadLoseSound();
    sounds->paddle = LoadPaddleSound();
    sounds->border = LoadBorderSound();
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

static Image LoadEmbeddedIcon(void) {
    return LoadImageFromMemory(".png", ico_png, ico_png_len);
}

static Sound LoadBorderSound(void) {
    Wave wave = LoadWaveFromMemory(".wav", border_wav, border_wav_len);
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

static Sound LoadLoseSound(void) {
    Wave wave = LoadWaveFromMemory(".wav", lose_wav, lose_wav_len);
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

static Sound LoadPaddleSound(void) {
    Wave wave = LoadWaveFromMemory(".wav", paddle_wav, paddle_wav_len);
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

static Sound LoadStartSound(void) {
    Wave wave = LoadWaveFromMemory(".wav", start_wav, start_wav_len);
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

static Sound LoadWinSound(void) {
    Wave wave = LoadWaveFromMemory(".wav", win_wav, win_wav_len);
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}
