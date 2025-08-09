# BrickGame 1.0

Implementation of the game Tetris in the C programming language with a terminal user interface.

Made as part of a School 21 assignment.

> "'Tetris' is an inventory management survival horror game from the Soviet Union in 1984."
>
> --- suckerpinch

## Project structure

The project consists of a library that implements the logic of the game.
And a separate CLI application implementing the game's graphics and controls using the `ncurses` library.

## Building and running

If you're in a UNIX-y environment, you can run

```sh
make
```

to build the project, and

```sh
make run
```

to run it.

(You need to have `gcc` and `libncurses-dev` installed)

If you're on Windows you're on your own. The assignment required me to use a
Makefile and those aren't particularly cross platform.

## Differences from the original assignment submission

All School 21 owned materials, such as task specifications, were removed.

Directory structure was altered a bit. The original assignment wanted to have the Makefile
in the src directory. Here it was moved into the project root as I feel like that makes more sense.

Parts of the code were reworked slightly, compared to the original submission.
