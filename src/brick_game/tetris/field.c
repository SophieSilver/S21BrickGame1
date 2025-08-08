#include "field.h"

#include "piece.h"

bool field_line_filled(const PlayField *field, int row) {
    bool result = true;

    for (int j = 0; j < PLAYFIELD_COLS && result; j += 1) {
        result = result && field->blocks[row][j];
    }

    return result;
}

bool piece_collides_with_playfield(const PlayField *field, PlayedPiece piece) {
    PieceData piece_data = get_piece_data(piece.piece_type, piece.rotation);

    bool result = false;

    for (int i = 0; i < PIECE_ROWS && !result; i += 1) {
        for (int j = 0; j < PIECE_COLS && !result; j += 1) {
            int y = piece.y + i;
            int x = piece.x + j;

            if (x >= 0 && x < PLAYFIELD_COLS && y >= 0 && y < PLAYFIELD_ROWS) {
                result |= (field->blocks[y][x] && piece_data.blocks[i][j]);
            }
            // we allow y to be negative to allow a piece to rotate at the very top
            else if (x < 0 || x >= PLAYFIELD_COLS || y >= PLAYFIELD_ROWS) {
                result |= piece_data.blocks[i][j];
            }
        }
    }

    return result;
}

PlayField playfield_merge_piece(const PlayField *field, PlayedPiece piece) {
    PlayField result = *field;
    PieceData piece_data = get_piece_data(piece.piece_type, piece.rotation);

    for (int i = 0; i < PIECE_ROWS; i += 1) {
        for (int j = 0; j < PIECE_COLS; j += 1) {
            int y = piece.y + i;
            int x = piece.x + j;

            if (x >= 0 && x < PLAYFIELD_COLS && y >= 0 && y < PLAYFIELD_ROWS) {
                result.blocks[y][x] |= piece_data.blocks[i][j];
            }
        }
    }

    return result;
}
