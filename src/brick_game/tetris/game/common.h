//! @file common.h
//! Defines common functionality shared across different states.

#ifndef BRICK_GAME_V1_0_BRICK_GAME_TETRIS_GAME_COMMON_H_
#define BRICK_GAME_V1_0_BRICK_GAME_TETRIS_GAME_COMMON_H_

#include <stdlib.h>

#include "../game.h"

/// Generate a random piece type.
static inline PieceType generate_piece(void) {
  return rand() % PIECE_TYPE_COUNT;
}

/// Transition the current game state to the given one.
///
/// Resets the @ref GameData.timer.
///
/// @memberof GameData
static inline void transition_state(GameData *game_data, GameState new_state) {
  game_data->state = new_state;
  game_data->timer = 0;
}

/// Reset game state between play sessions.
///
/// Preserves top score so it does not need to be reloaded.
///
/// @memberof GameData
static inline void reset_game_data(GameData *game_data) {
  // don't reset the top score
  int top_score = game_data->top_score;
  *game_data = (GameData){0};
  game_data->top_score = top_score;

  game_data->played_piece.x = -4;
  game_data->played_piece.y = -4;
}

#endif  // BRICK_GAME_V1_0_BRICK_GAME_TETRIS_GAME_UTILS_H_
