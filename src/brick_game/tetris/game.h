//! @file game.h
//! Main entry point of the library.
//!
//! Defines the main data structure of the game --- @ref GameData
//! and possible operations on it.

#ifndef BRICK_GAME_V1_0_BRICK_GAME_TETRIS_GAME_H_
#define BRICK_GAME_V1_0_BRICK_GAME_TETRIS_GAME_H_

#include <stdbool.h>

#include "field.h"
#include "piece.h"

/// The maximum level the player can reach.
#define MAX_LEVEL 10

/// Enumeration of all states of the game's Finite State Machine.
typedef enum {
    /// Start screen of the game.
    ///
    /// Transitions to @ref SPAWN_START when the user action @ref Start is
    /// received.
    START,

    /// Spawn a random first piece and generate a random next piece.
    ///
    /// Different from @ref SPAWN because the spawned piece is random and not the
    /// next piece.
    ///
    /// Entered from @ref START and @ref GAME_OVER for the first spawn of the
    /// game.
    ///
    /// Transitions to @ref MOVING after the piece has been spawned.
    SPAWN_START,

    /// Spawn the next piece and generate a new random next piece.
    ///
    /// Applies spawn delay proportional to the distance of the attached piece
    /// from the ground. The higher the piece was attached, the longer the delay,
    /// to provide the player with ample time to think when they are close
    /// to losing.
    ///
    /// Transitions to @ref GAME_OVER if there is not enough space for the spawned
    /// piece.
    ///
    /// Otherwise, transitions to @ref MOVING after the piece has been spawned.
    SPAWN,

    /// Main state of the game, the player is in control of the piece.
    ///
    /// The played piece is rotated and moved sideways or downwards according to
    /// player's inputs.
    ///
    /// The piece is also automatically moved downwards at fixed intervals. The
    /// intervals get progressively shorter at higher levels.
    ///
    /// Transitions to @ref ATTACHING when the piece touches the ground.
    MOVING,

    /// The played piece is attached to the field.
    ///
    /// Transitions to @ref CLEARING if the attached piece completes one or more
    /// lines.
    ///
    /// Otherwise, transitions to @ref SPAWN.
    ATTACHING,

    /// Full lines are cleared and points are awarded.
    ///
    /// Plays a quick animation of the lines clearing and moves the lines above
    /// down.
    ///
    /// Points are awarded based on amount of lines cleared according to the table
    /// below:
    ///
    /// | Lines Cleared | Points Awarded |
    /// | ------------- | -------------- |
    /// | 1             | 100            |
    /// | 2             | 300            |
    /// | 3             | 700            |
    /// | 4             | 1500           |
    ///
    /// Transitions to @ref SPAWN
    CLEARING,

    /// Game over screen of the game.
    ///
    /// Transitions to @ref SPAWN_START when the user action @ref Start is
    /// received.
    GAME_OVER,
} GameState;

/// A bitfield describing active user inputs.
typedef struct {
    bool start;
    bool pause;
    bool terminate;
    bool left;
    bool right;
    bool down;
    bool rotate;
} Inputs;

/// A list of indices of lines that are detected to be full.
///
/// The maximum amount of filled lines is 4.
typedef struct {
    /// Array of indices of filled lines.
    int lines[4];
    /// Length of the initialized portions of the @ref lines array.
    int len;
} FilledLines;

/// A structure containing all data about the current game session.
typedef struct {
    /// Current state of the game's state machine.
    GameState state;

    /// Current Playfield.
    ///
    /// Does not include the blocks of the played piece.
    PlayField field;

    /// Information about the currently played piece.
    PlayedPiece played_piece;

    /// The type of the next piece to spawn.
    PieceType next_piece;

    /// Information about cleared lines.
    ///
    /// Only used in @ref ATTACHING and @ref CLEARING
    FilledLines filled_lines;

    /// Counter used for timing different things.
    ///
    /// Used to time piece falling, clearing animation and spawn delay.
    ///
    /// Reset to 0 on state transition.
    int timer;

    /// The y position of the last attached piece.
    ///
    /// Used to calculate spawn delay.
    int piece_land_y;

    /// Current score.
    int score;

    /// Current highscore.
    ///
    /// It is stored in a file and thus, persists through games.
    int top_score;

    /// Inputs that are being pressed this frame.
    Inputs inputs;

    /// Inputs that are being held down this frame.
    ///
    /// When held, `left`, `down`, and `up` will be re-triggered on each frame.
    /// Other held inputs are ignored.
    Inputs held_inputs;

    /// Indicates if the game is currently paused.
    bool paused;

    /// Indicates if the @ref top_score has been loaded from the file yet.
    ///
    /// If this is false, @ref top_score will be loaded during the next update.
    bool top_score_loaded;
} GameData;

/// Advance the game by one frame.
///
/// If the game is not paused, the code for the current FSM state will be
/// executed.
///
/// @memberof GameData
void update_game_data(GameData *game_data);

/// Get the copy of the current playfield combined with the current piece.
///
/// A wrapper for @ref playfield_merge_piece.
///
/// @memberof GameData
PlayField get_combined_game_field(const GameData *game_data);

/// Get the current level of the game.
///
/// Level is fully depended on score. Starting at 1, it increases by one for
/// every 600 points earned, until level 10.
///
/// Because level depends on score, it is not stored, instead it can just be
/// calculated.
///
/// @memberof GameData
int get_level(const GameData *game_data);

#endif  // #ifndef BRICK_GAME_V1_0_BRICK_GAME_TETRIS_STATE_H_
