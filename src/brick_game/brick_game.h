//! @file brick_game.h
//! Main interface between the game logic and the GUI.

#ifndef BRICK_GAME_V1_0_BRICK_GAME_BRICK_GAME_H_
#define BRICK_GAME_V1_0_BRICK_GAME_BRICK_GAME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef enum GameStatus {
    GAME_STATUS_RUNNING,
    GAME_STATUS_PAUSED,
    GAME_STATUS_GAME_OVER,
    GAME_STATUS_START,
} GameStatus;

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
} UserAction;

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
    /// The status of the game.
    ///
    /// @see GameStatus
    GameStatus status;
} GameInfo;

/// Opaque structure containing the instance of the game data.
struct GameInstance;
typedef struct GameInstance GameInstance;

/// Create a new @ref GameInstance.
GameInstance *game_instance_create(void);

/// Destroy a @ref GameInstance and release its resources.
void game_instance_destroy(GameInstance *instance);

/// Register a user input.
///
/// @param action --- User input to register
///
/// @memberof GameInstance
void user_input(GameInstance *instance, UserAction action);

/// Advance the game state by one frame.
///
/// @return Information about the current state of the game.
///
/// @memberof GameInstance
GameInfo update_current_state(GameInstance *instance);

#ifdef __cplusplus
}
#endif

#endif  // BRICK_GAME_V1_0_BRICK_GAME_BRICK_GAME_H_
