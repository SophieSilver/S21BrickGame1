#include "piece.h"

/// properties of each unique piece type
typedef struct {
    /// blocks of a piece in the default rotation
    ///
    /// The block [1][2] is considered the center of rotation (except for the I
    /// piece) As per https://tetris.wiki/Original_Rotation_System
    int blocks[PIECE_ROWS][PIECE_COLS];
    /// Amount of unique rotations
    int unique_rotations;
    /// How many blocks do we need to lower the piece for it to be fully
    /// in the view when spawned
    int spawn_height_offset;
} PieceTypeProperties;

/// An internal array mapping `PieceType` to `PieceTypeProperties`
static const PieceTypeProperties pieces[PIECE_TYPE_COUNT] = {
    // O piece
    {
        .blocks =
            {
                {0, 0, 0, 0},
                {0, 1, 1, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0},
            },
        .unique_rotations = 1,
        .spawn_height_offset = 2,
    },
    // I piece
    {
        .blocks =
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
            },
        .unique_rotations = 2,
        // 1 because the rotation code places the I piece in horizontal position
        // 1 block lower
        .spawn_height_offset = 1,
    },
    // S piece
    {
        .blocks =
            {
                {0, 0, 0, 0},
                {0, 0, 1, 1},
                {0, 1, 1, 0},
                {0, 0, 0, 0},
            },
        .unique_rotations = 2,
        .spawn_height_offset = 2,
    },
    // Z piece
    {
        .blocks =
            {
                {0, 0, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 1, 1},
                {0, 0, 0, 0},
            },
        .unique_rotations = 2,
        .spawn_height_offset = 2,
    },
    // J piece
    {
        .blocks =
            {
                {0, 0, 0, 0},
                {0, 1, 1, 1},
                {0, 0, 0, 1},
                {0, 0, 0, 0},
            },
        .unique_rotations = 4,
        .spawn_height_offset = 2,
    },
    // L piece
    {
        .blocks =
            {
                {0, 0, 0, 0},
                {0, 1, 1, 1},
                {0, 1, 0, 0},
                {0, 0, 0, 0},
            },
        .unique_rotations = 4,
        .spawn_height_offset = 2,
    },
    // T piece
    {
        .blocks =
            {
                {0, 0, 0, 0},
                {0, 1, 1, 1},
                {0, 0, 1, 0},
                {0, 0, 0, 0},
            },
        .unique_rotations = 4,
        .spawn_height_offset = 2,
    },
};

static inline void rotate_coordinates(int true_rotation, int *restrict ri, int *restrict rj) {
    // center on the pivot point
    *ri -= 1;
    *rj -= 2;

    if (true_rotation > 1) {
        *ri = -*ri;
        *rj = -*rj;
    }

    if (true_rotation % 2 == 1) {
        int temp = *ri;
        *ri = *rj;
        *rj = -temp;
    }

    // uncenter the pivot point
    *ri += 1;
    *rj += 2;
}

PieceData get_piece_data(PieceType piece_type, int rotation) {
    PieceTypeProperties properties = pieces[piece_type];
    PieceData result = {0};
    // change rotation direction to clockwise (I like it better)
    rotation = (4 - rotation) % 4;
    int true_rotation = rotation % properties.unique_rotations;

    for (int i = 0; i < PIECE_ROWS; i += 1) {
        for (int j = 0; j < PIECE_COLS; j += 1) {
            int ri = i;
            int rj = j;
            rotate_coordinates(true_rotation, &ri, &rj);

            // make the I piece in horizontal orientation sit lower
            // (mirroring NES Tetris)
            if (piece_type == I_PIECE && true_rotation == 0) {
                ri -= 1;
            }

            if (ri < 0 || ri >= PIECE_ROWS || rj < 0 || rj >= PIECE_COLS) {
                result.blocks[i][j] = 0;
            } else {
                result.blocks[i][j] = properties.blocks[ri][rj];
            }
        }
    }

    return result;
}

int piece_spawn_height_offset(PieceType piece_type) {
    return pieces[piece_type].spawn_height_offset;
}
