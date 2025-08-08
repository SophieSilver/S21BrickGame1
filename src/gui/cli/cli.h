//! @file cli.h
//! The main interface of the CLI.

#ifndef BRICK_GAME_V1_0_GUI_CLI_CLI_H_
#define BRICK_GAME_V1_0_GUI_CLI_CLI_H_

#include <stdbool.h>

#include "../../brick_game/brick_game.h"
#include "view.h"

/// CLI Controller.
///
/// Runs the main loop, collects inputs and passes them into the library, passes
/// information about the game to @ref CliView.
typedef struct {
    /// Information about the game.
    ///
    /// This is the "model" that is being rendered.
    GameInfo_t game_info;

    /// CLI View that is responsible for rendering @ref game_info.
    CliView view;

    /// Flag, signifying if the controller should continue running the main loop,
    /// or exit.
    bool running;
} CliController;

/// Create a new CliController.
///
/// @memberof CliController
CliController cli_controller_create(void);

/// Run the main application loop.
///
/// @memberof CliController
void cli_controller_run_main_loop(CliController *controller);

/// Destroy CliController and release its resources.
///
/// @memberof CliController
void cli_controller_destroy(CliController controller);

#endif  // BRICK_GAME_V1_0_GUI_CLI_CLI_H_
