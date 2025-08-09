#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../brick_game.h"
#include "field.h"
#include "game.h"
#include "piece.h"
#include "utils.h"

struct GameInstance {
    GameData game_data;

    // data used to populate GameInfo
    PieceData next_piece_data;
    PlayField combined_field;
    int *field_matrix[PLAYFIELD_ROWS];
    int *next_matrix[PIECE_ROWS];
};

static inline GameInfo get_game_info_from_instance(GameInstance *instance) {
    const GameData *game_data = &instance->game_data;

    instance->combined_field = get_combined_game_field(game_data);
    array2d_to_matrix((int *)instance->combined_field.blocks, PLAYFIELD_ROWS, PLAYFIELD_COLS, instance->field_matrix);

    instance->next_piece_data = get_piece_data(game_data->next_piece, 0);
    array2d_to_matrix((int *)instance->next_piece_data.blocks, PIECE_ROWS, PIECE_COLS, instance->next_matrix);

    GameStatus status = GAME_STATUS_RUNNING;

    if (game_data->paused) {
        status = GAME_STATUS_PAUSED;
    } else if (game_data->state == START) {
        status = GAME_STATUS_START;
    } else if (game_data->state == GAME_OVER) {
        status = GAME_STATUS_GAME_OVER;
    }

    GameInfo game_info = {
        .field = instance->field_matrix,
        .next = instance->next_matrix,
        .score = game_data->score,
        .high_score = game_data->top_score,
        .level = get_level(game_data),
        .status = status,
    };

    return game_info;
}

void user_input(GameInstance *game_instance, UserAction action) {
    Inputs *inputs = &game_instance->game_data.inputs;

    switch (action) {
        case Start:
            inputs->start = true;
            break;

        case Pause:
            inputs->pause = true;
            break;

        case Terminate:
            inputs->terminate = true;
            break;

        case Left:
            inputs->left = true;
            break;

        case Right:
            inputs->right = true;
            break;

        case Down:
            inputs->down = true;
            break;

        case Action:
            inputs->rotate = true;
            break;

        case Up:
            break;
    }
}

GameInstance *game_instance_create(void) {
    GameInstance *instance = malloc(sizeof(GameInstance));
    // All of this is ZII
    *instance = (GameInstance) {0};

    return instance;
}

void game_instance_destroy(GameInstance *instance) {
    free(instance);
}

GameInfo update_current_state(GameInstance *instance) {
    GameData *game_data = &instance->game_data;

    update_game_data(game_data);
    return get_game_info_from_instance(instance);
}
