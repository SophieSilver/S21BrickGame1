#include "moving.h"

#include "common.h"

static inline bool try_rotate_piece(GameData *game_data) {
    PlayedPiece piece = game_data->played_piece;
    piece.rotation = (piece.rotation + 1) % 4;

    bool succeeded = false;
    if (!piece_collides_with_playfield(&game_data->field, piece)) {
        game_data->played_piece = piece;
        succeeded = true;
    }
    return succeeded;
}

static inline bool try_shift_piece(GameData *game_data, int dx) {
    PlayedPiece piece = game_data->played_piece;
    piece.x += dx;

    bool succeeded = false;
    if (!piece_collides_with_playfield(&game_data->field, piece)) {
        game_data->played_piece = piece;
        succeeded = true;
    }
    return succeeded;
}

static inline bool try_lower_piece(GameData *game_data) {
    PlayedPiece piece = game_data->played_piece;
    piece.y += 1;

    bool succeeded = false;
    if (!piece_collides_with_playfield(&game_data->field, piece)) {
        game_data->played_piece = piece;
        succeeded = true;
    }

    return succeeded;
}

void handle_moving(GameData *game_data) {
    game_data->timer += 1;

    if (game_data->inputs.rotate) {
        try_rotate_piece(game_data);
    }
    bool on_ground = false;
    int drop_interval = MAX_LEVEL - get_level(game_data) + 2;

    if (game_data->timer >= drop_interval || game_data->inputs.down) {
        game_data->timer = 0;
        on_ground = !try_lower_piece(game_data);
    }
    if (!on_ground) {
        if (game_data->inputs.left) {
            try_shift_piece(game_data, -1);
        }

        if (game_data->inputs.right) {
            try_shift_piece(game_data, 1);
        }
    } else {
        transition_state(game_data, ATTACHING);
    }
}
