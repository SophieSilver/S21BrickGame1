#include "start.h"

#include "common.h"
#include "saving.h"

void handle_start(GameData *game_data) {
    // avoid a visual artifact where the piece is briefly visible before being
    // spawned
    game_data->played_piece.x = -4;
    game_data->played_piece.y = -4;

    if (game_data->inputs.start) {
        // the next piece is generated in the START state instead of SPAWN_START
        // to avoid visual artifacts with the next piece "blinking" for one frame
        game_data->next_piece = generate_piece();
        transition_state(game_data, SPAWN_START);
    }
}

void handle_game_over(GameData *game_data) {
    if (game_data->inputs.start) {
        save_top_score(game_data->top_score);
        reset_game_data(game_data);
        game_data->next_piece = generate_piece();
        transition_state(game_data, SPAWN_START);
    }
}
