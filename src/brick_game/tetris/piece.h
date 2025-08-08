//! @file piece.h
//! Defines pieces and operations on them.

#ifndef BRICK_GAME_V1_0_BRICK_GAME_TETRIS_PIECE_H_
#define BRICK_GAME_V1_0_BRICK_GAME_TETRIS_PIECE_H_

/// Number of rows in @ref PieceData.blocks
#define PIECE_ROWS 4
/// Number of columns in @ref PieceData.blocks
#define PIECE_COLS 4

#define PIECE_TYPE_COUNT 7

/// An array of blocks the piece consists of.
typedef struct {
    /// A 4 by 4 matrix of blocks that a piece consists of.
    ///
    /// 0 represents an empty cell and 1 represents a filled cell.
    int blocks[PIECE_ROWS][PIECE_COLS];
} PieceData;

/// Possible piece types.
typedef enum {
    O_PIECE = 0,
    I_PIECE = 1,
    S_PIECE = 2,
    Z_PIECE = 3,
    J_PIECE = 4,
    L_PIECE = 5,
    T_PIECE = 6,
} PieceType;

/// State of the currently played piece
typedef struct {
    /// X offset of the upper left corner of the piece data
    int x;
    /// Y offset of the upper left corner of the piece data
    int y;
    /// Rotation of the piece, [0; 3]
    int rotation;
    /// The type of the piece being played
    PieceType piece_type;
} PlayedPiece;

/// Get the block data from piece type and rotation.
PieceData get_piece_data(PieceType piece_type, int rotation);

/// Get the offset at which we should spawn the piece on the field.
///
/// The offset should be such, that in the default orientation the piece
/// occupies the top row of the field
int piece_spawn_height_offset(PieceType piece_type);

#endif
