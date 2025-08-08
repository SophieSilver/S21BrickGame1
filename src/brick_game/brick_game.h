//! @file brick_game.h
//! Main interface between the game logic and the GUI.

#ifndef BRICK_GAME_V1_0_BRICK_GAME_BRICK_GAME_H_
#define BRICK_GAME_V1_0_BRICK_GAME_BRICK_GAME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// The provided interface does not include variables for game state, like
// paused, start, game-over, etc.
// So we stuff these values into the `pause` field

#define RUNNING 0
#define PAUSED 1
#define GAME_OVER_SCREEN 2
#define START_SCREEN 3

#define FIELD_ROWS 20
#define FIELD_COLS 10
#define NEXT_ROWS 4
#define NEXT_COLS 4

/// A player action
typedef enum UserAction {
    /// Start the game
    Start,
    /// Pause the game
    Pause,
    /// Stop the game
    Terminate,
    /// Left D-pad button
    Left,
    /// Right D-pad button
    Right,
    /// Up D-pad button
    Up,
    /// Down D-pad button
    Down,
    /// Action button
    Action
} UserAction_t;

/// Information about the current state of the game.
typedef struct GameInfo {
    /// 20x10 Matrix of representing the game field
    /// Each cell is either 0 - empty, or 1 - filled
    int **field;
    /// a 4x4 Matrix representing the next piece
    /// Each cell is either 0 - empty, or 1 - filled
    int **next;
    /// The current score
    int score;
    /// The highest score ever achieved
    int high_score;
    /// The current level, 1 - 10
    int level;
    /// The current speed of the game, 1 - 10
    int speed;
    /// The pause state of the game.
    ///
    /// One of:
    /// - 0 = @ref RUNNING
    /// - 1 = @ref PAUSED
    /// - 2 = @ref GAME_OVER_SCREEN
    /// - 3 = @ref START_SCREEN
    int pause;
} GameInfo_t;

/// Register a user input.
///
/// @param action --- User input to register
///
/// @param hold --- Whether the input should be pressed once or held down
/// continuously. If `true`, the input will be considered held down until a call
/// to @ref userInput with the same action and `hold` = `false` is received.
void userInput(UserAction_t action, bool hold);

/// Advance the game state by one frame.
///
/// @return Information about the current state of the game.
GameInfo_t updateCurrentState(void);

#ifdef __cplusplus
}
#endif

#endif  // BRICK_GAME_V1_0_BRICK_GAME_BRICK_GAME_H_
