# pong

## Description

A simple implementation of the classic game Pong in C using the [raylib](https://www.raylib.com/index.html) library.

Specific information about the Pong game itself can be found [here](https://en.wikipedia.org/wiki/Pong).

## Screenshots

![Gameplay](/screenshots/gameplay.png "Gameplay")

## Game features

- Minimalist design - a tribute to the classic game 💎

- Endless game mode with bot - you set your own victory points 😎

- There are sounds! 🎶

- You can pause the game ⏯️ ...

- ... or restart the game by resetting the score. 💫

- It's free 🆓

## Project features

- The game is completely written in C99

- No game engine - just a single [raylib](https://www.raylib.com/index.html) library

- The [raylib](https://www.raylib.com/index.html) library is pulled at build time from its [official repository on Github](https://github.com/raysan5/raylib).

- Supported building of the game for Windows and GNU/Linux


## Usage

For quick playability, go to the [releases](https://github.com/artem-sharipov/pong/releases) page and download the appropriate archive for your system. Unzip the archive and run the executable. Enjoy the game!

If you want to build the game yourself - go to the next section.

## Build

To build, clone the repository (or its fork) to yourself locally. Since cmake is used, the configuration and build steps are identical for Windows and GNU/Linux:

1. Go to the `pong` repository;

2. Run the command to configure the build:

    ```
    cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
    ```

3. Once the configuration is complete, start the build itself:

    ```
    cmake --build build
    ```
    Build tools are platform specific. If these tools are missing, the build will fail. In case of a build fail, you have to read the error messages.

After a successful build, the game needs to be deployed - go to the next section.

## Deploy

 Simply, you need to put together the assembled game executable and the `resources` folder. This process is platform-specific, and there are two scripts to automate it: `deploy.bat` for Windows and `deploy.sh` for GNU/Linux. The following subsections describe how to run scripts on different systems.

 As a result of successful scripting, the `deploy` folder will appear, in which the game executable and `resources` folder are stored. You can move this folder to a convenient location and rename it. You should run the game from this folder.

### Deploy on Windows

Run `deploy.bat` through the GUI (double-click) or terminal.

### Deploy on GNU/Linux

Make `deploy.sh` executable:
    
```bash
chmod +x deploy.sh
```
Run `deploy.sh`:

```bash
./deploy.sh
```
## Plans

- Refine in-game controls

- Support for playing the game to a winning score

- Add local multiplayer (on a keyboard)

- Network multiplayer support