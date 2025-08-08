#include "clearing.h"

#include <string.h>

#include "../utils.h"
#include "common.h"

#define SCORE_PER_LEVEL 600

static const int score_per_lines[5] = {
    0,
    100,
    300,
    700,
    1500,
};

static inline void award_points(GameData *game_data) {
    int points = score_per_lines[game_data->filled_lines.len];

    game_data->score += points;
    game_data->top_score = MAX(game_data->score, game_data->top_score);
}

static inline void clear_filled_lines(GameData *game_data) {
    size_t row_size = sizeof(game_data->field.blocks[0]);
    FilledLines *filled_lines = &game_data->filled_lines;

    for (int i = 0; i < filled_lines->len; i += 1) {
        int j = filled_lines->len - i - 1;
        int start_line = 0;
        if (j > 0) {
            start_line = filled_lines->lines[j - 1];
        }
        int end_line = filled_lines->lines[j];

        int span = end_line - start_line;
        size_t shift = i + 1;
        void *src = game_data->field.blocks[start_line];
        void *dest = game_data->field.blocks[start_line + shift];
        size_t size = span * row_size;

        memmove(dest, src, size);
    }

    // set top lines to 0
    memset(&game_data->field.blocks[0], 0, filled_lines->len * row_size);
}

static inline void animate_clearing(GameData *game_data) {
    FilledLines *filled_lines = &game_data->filled_lines;

    for (int i = 0; i < filled_lines->len; i += 1) {
        int *line = game_data->field.blocks[filled_lines->lines[i]];
        for (int j = 0; j < PLAYFIELD_COLS; j += 1) {
            int distance_from_center = (PLAYFIELD_COLS / 2 - 1) - MIN(j, PLAYFIELD_COLS - j - 1);

            line[j] = distance_from_center > game_data->timer;
        }
    }
}

void handle_clearing(GameData *game_data) {
    if (game_data->timer < PLAYFIELD_COLS / 2) {
        animate_clearing(game_data);
        game_data->timer += 1;
    } else {
        award_points(game_data);

        clear_filled_lines(game_data);
        game_data->filled_lines = (FilledLines) {0};
        transition_state(game_data, SPAWN);
    }
}
