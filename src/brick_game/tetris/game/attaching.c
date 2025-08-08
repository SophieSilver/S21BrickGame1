#include "attaching.h"

#include "common.h"

static inline bool find_filled_lines(GameData *game_data) {
    FilledLines *filled_lines = &game_data->filled_lines;
    filled_lines->len = 0;
    for (int i = 0; i < PLAYFIELD_ROWS && filled_lines->len < 4; i += 1) {
        if (field_line_filled(&game_data->field, i)) {
            filled_lines->lines[filled_lines->len] = i;
            filled_lines->len += 1;
        }
    }

    return filled_lines->len > 0;
}

void handle_attaching(GameData *game_data) {
    game_data->field = playfield_merge_piece(&game_data->field, game_data->played_piece);

    game_data->piece_land_y = game_data->played_piece.y;
    // hide the played piece, avoids visual artifacts when clearing
    game_data->played_piece.x = -4;
    game_data->played_piece.y = -4;

    bool full_lines_detected = find_filled_lines(game_data);

    if (full_lines_detected) {
        transition_state(game_data, CLEARING);
    } else {
        transition_state(game_data, SPAWN);
    }
}
