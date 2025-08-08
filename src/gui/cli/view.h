//! @file view.h
//! The CLI View.

#ifndef BRICK_GAME_V1_0_GUI_CLI_VIEW_H_
#define BRICK_GAME_V1_0_GUI_CLI_VIEW_H_

#include <ncurses.h>
#include <stdint.h>

#include "../../brick_game/brick_game.h"

/// CLI View responsible for rendering the game information to the screen
typedef struct {
    WINDOW *field_win;
    WINDOW *side_win;
    WINDOW *next_win;
    WINDOW *start_win;
    WINDOW *game_over_win;
    WINDOW *pause_win;
    WINDOW *controls_win;
    WINDOW *top_score_win;
    WINDOW *score_win;
    WINDOW *level_win;
    WINDOW *speed_win;

    /// Frame counter.
    ///
    /// Used for timing blinking elements.
    uint32_t frame_count;  // unsigned so no UB on overflow
} CliView;

/// Create the CLI View.
///
/// @memberof CliView
CliView cli_view_create(void);

/// Rerender the view with the new model.
///
/// @memberof CliView
void cli_view_update(CliView *view, GameInfo_t game_info);

/// Destroy the CLI View.
///
/// @memberof CliView
void cli_view_destroy(CliView view);

#endif  // BRICK_GAME_V1_0_GUI_CLI_VIEW_H_
