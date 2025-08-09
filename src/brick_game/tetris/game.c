#include "game.h"

#include "game/attaching.h"
#include "game/clearing.h"
#include "game/common.h"
#include "game/moving.h"
#include "game/saving.h"
#include "game/spawn.h"
#include "game/start.h"
#include "utils.h"

static inline void handle_pausing(GameData *game_data) {
    if (game_data->state != START && game_data->state != GAME_OVER) {
        if (!game_data->paused && game_data->inputs.pause) {
            game_data->paused = true;
        }

        else if (game_data->paused && (game_data->inputs.pause || game_data->inputs.start)) {
            game_data->paused = false;
        }
    } else {
        // clear the pause flag if it found its way here
        game_data->paused = false;
    }
}

static inline void handle_terminating(GameData *game_data) {
    if (game_data->inputs.terminate) {
        save_top_score(game_data->top_score);
        reset_game_data(game_data);
        transition_state(game_data, START);
    }
}

static inline void handle_state(GameData *game_data) {
    switch (game_data->state) {
        case START:
            handle_start(game_data);
            break;

        case SPAWN_START:
            handle_spawn_start(game_data);
            break;

        case SPAWN:
            handle_spawn(game_data);
            break;

        case MOVING:
            handle_moving(game_data);
            break;

        case ATTACHING:
            handle_attaching(game_data);
            break;

        case CLEARING:
            handle_clearing(game_data);
            break;

        case GAME_OVER:
            handle_game_over(game_data);
            break;
    }
}

void update_game_data(GameData *game_data) {
    if (!game_data->top_score_loaded) {
        game_data->top_score = load_top_score();
        game_data->top_score_loaded = true;
    }

    handle_pausing(game_data);

    if (!game_data->paused) {
        handle_state(game_data);
    }

    handle_terminating(game_data);

    game_data->inputs = (Inputs) {0};
}

PlayField get_combined_game_field(const GameData *game_data) {
    return playfield_merge_piece(&game_data->field, game_data->played_piece);
}

int get_level(const GameData *game_data) {
    return 1 + MIN(MAX_LEVEL - 1, game_data->score / 600);
}
