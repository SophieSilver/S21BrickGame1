//! @file field.h
//! Defines the playfield and operations on it.

#ifndef BRICK_GAME_V1_0_BRICK_GAME_TETRIS_FIELD_H_
#define BRICK_GAME_V1_0_BRICK_GAME_TETRIS_FIELD_H_

#include <stdbool.h>

#include "piece.h"

/// Number of rows in the @ref PlayField
#define PLAYFIELD_ROWS 20
/// Number of columns in the @ref PlayField
#define PLAYFIELD_COLS 10

/// State of the playfield
typedef struct {
    /// A Matrix of cells comprising the field.
    ///
    /// Each cell is either empty (0) or filled (1).
    int blocks[PLAYFIELD_ROWS][PLAYFIELD_COLS];
} PlayField;

/// Check if all cells on the line at a given index are filled.
///
/// @memberof PlayField
bool field_line_filled(const PlayField *field, int index);

/// Check if the piece collides with the playfield
///
/// @memberof PlayField
bool piece_collides_with_playfield(const PlayField *field, PlayedPiece piece);

/// Create a copy of the playfield and merge the piece data into it
///
/// @memberof PlayField
PlayField playfield_merge_piece(const PlayField *field, PlayedPiece piece);

#endif  // BRICK_GAME_V1_0_BRICK_GAME_TETRIS_FIELD_H_
