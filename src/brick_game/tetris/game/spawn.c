#include "spawn.h"

#include "../utils.h"
#include "common.h"

#define SPAWN_X 3

static inline void spawn_piece(GameData *game_data, PieceType piece) {
    PlayedPiece played_piece = {
        .x = SPAWN_X,
        .y = piece_spawn_height_offset(piece) - 3,
        .rotation = 0,
        .piece_type = piece,
    };
    game_data->played_piece = played_piece;
}

void handle_spawn_start(GameData *game_data) {
    spawn_piece(game_data, generate_piece());

    transition_state(game_data, MOVING);
}

static inline int calculate_spawn_delay(int piece_land_y) {
    int piece_bottom = piece_land_y + 4;
    piece_bottom = MIN(20, MAX(0, piece_bottom));

    int distance_from_ground = 20 - piece_bottom;

    return 2 + distance_from_ground / 3;
}

void handle_spawn(GameData *game_data) {
    if (game_data->timer < calculate_spawn_delay(game_data->piece_land_y)) {
        // add a delay proportional to the height where the piece landed to give
        // the player some breathing room
        game_data->timer += 1;
    } else {
        spawn_piece(game_data, game_data->next_piece);
        game_data->next_piece = generate_piece();

        if (piece_collides_with_playfield(&game_data->field, game_data->played_piece)) {
            transition_state(game_data, GAME_OVER);
        } else {
            transition_state(game_data, MOVING);
        }
    }
}
