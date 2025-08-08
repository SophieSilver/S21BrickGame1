#include <stdbool.h>
#include <stddef.h>
#include <threads.h>

#include "../brick_game.h"
#include "field.h"
#include "game.h"
#include "piece.h"
#include "utils.h"

static inline GameData *get_game_data(void) {
    static thread_local GameData game_data = {0};
    return &game_data;
}

static inline GameInfo_t game_data_to_game_info(const GameData *data) {
    // thread locals so we have some thread safety
    static thread_local GameInfo_t game_info = {0};
    static thread_local int *field_matrix[PLAYFIELD_ROWS] = {0};
    static thread_local int *next_matrix[PIECE_ROWS] = {0};
    static thread_local PieceData next_piece_data = {0};
    static thread_local PlayField combined_field = {0};

    combined_field = get_combined_game_field(data);
    array2d_to_matrix((int *)combined_field.blocks, PLAYFIELD_ROWS, PLAYFIELD_COLS, field_matrix);

    next_piece_data = get_piece_data(data->next_piece, 0);
    array2d_to_matrix((int *)next_piece_data.blocks, PIECE_ROWS, PIECE_COLS, next_matrix);

    game_info.field = field_matrix;
    game_info.next = next_matrix;

    const GameData *game_data = get_game_data();

    game_info.pause = 0;
    if (game_data->paused) {
        game_info.pause = PAUSED;
    } else if (game_data->state == START) {
        game_info.pause = START_SCREEN;
    } else if (game_data->state == GAME_OVER) {
        game_info.pause = GAME_OVER_SCREEN;
    }

    game_info.score = game_data->score;
    game_info.high_score = game_data->top_score;
    game_info.level = get_level(game_data);
    game_info.speed = game_info.level;

    return game_info;
}

static inline void apply_input(GameData *game_data, size_t offset, bool hold) {
    bool *inputs_base = (bool *)&game_data->inputs;
    bool *held_inputs_base = (bool *)&game_data->held_inputs;

    bool *input = inputs_base + offset;
    bool *held_input = held_inputs_base + offset;

    // if was holding or started holding, only assign to held_input
    // this prevents a "button up" being registered as a press.
    if (hold || *held_input) {
        *held_input = hold;
    } else {
        *input = true;
    }
}

void userInput(UserAction_t action, bool hold) {
    GameData *game_data = get_game_data();
    size_t offset = 0;
    switch (action) {
        case Start:
            offset = offsetof(Inputs, start);
            break;

        case Pause:
            offset = offsetof(Inputs, pause);
            break;

        case Terminate:
            offset = offsetof(Inputs, terminate);
            break;

        case Left:
            offset = offsetof(Inputs, left);
            break;

        case Right:
            offset = offsetof(Inputs, right);
            break;

        case Down:
            offset = offsetof(Inputs, down);
            break;

        case Action:
            offset = offsetof(Inputs, rotate);
            break;

        case Up:
            break;
    }

    if (action != Up) {
        apply_input(game_data, offset, hold);
    }
}

GameInfo_t updateCurrentState(void) {
    GameData *game_data = get_game_data();

    update_game_data(game_data);
    return game_data_to_game_info(game_data);
}
