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

## Build & Deploy

Building and deploying the game is done by running just one script - `build_and_deploy.py`. This script is fully cross-platform for GNU/Linux and Windows. The script requires `Python 3.8+`.

This script deletes the `build` folder (configuration and build results) and the `deploy` folder (game resources and executable), and then performs the configuration and build using `cmake 3.15+`.

After building, deployment is performed - a `deploy` folder is created, in which the game `resources` folder and the executable folder are placed (in the current implementation, this folder structure must be followed in order for the resources to be processed correctly by the game). At the end of deployment, a platform-dependent archive with the game and its resources is created. The archives in the [releases](https://github.com/artem-sharipov/pong/releases) section are obtained in this way.

To run a script in GNU/Linux, you first need to make it executable with the following command:

```bash
chmod +x build_and_deploy.py
```

And the launch itself is done like this:

```bash
./build_and_deploy.py
```
In Windows, open a terminal (cmd or PowerShell) in the project folder and run:
```powershell
python build_and_deploy.py
```
Debugging messages about configuration, build, and deployment may appear in the terminal window from the beginning of the script run. If an error occurs, you can see it by the messages in the terminal. The result of the script build will be a platform-specific archive. After successful build, the terminal can be closed, if it has not closed itself.

You might wonder why all stages of preparing the game for use from configuration to deployment are performed by one script, and it does it always cleanly, although, for example, some things may not be rebuilt to save time. First, the point is that the creation of the version string (which the game code uses) occurs only at the configuration stage, so configuring the build once and simply rebuilding the project will not update the version string, in particular the build date and repository status, since this information is obtained only at the configuration stage. We can consider the current solution to be a crutch to some extent. Secondly, a complete clean process guarantees that everything will be built completely as it should be. This build and deployment process can be changed in the future.
    
## Plans

- Refine in-game controls

- Support for playing the game to a winning score

- Add local multiplayer (on a keyboard)

- Network multiplayer support