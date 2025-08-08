//! @file saving.h
//! Implements top score saving functionality

#ifndef BRICK_GAME_V1_0_BRICK_GAME_TETRIS_GAME_SAVING_H_
#define BRICK_GAME_V1_0_BRICK_GAME_TETRIS_GAME_SAVING_H_

/// Load top score from a file on disk.
///
/// Returns 0 if the top score file is missing, corrupted or cannot be read.
int load_top_score(void);

/// Save the top score to a file on disk.
void save_top_score(int top_score);

#endif
