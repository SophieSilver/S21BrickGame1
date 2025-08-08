#include "saving.h"

#include <stdio.h>
// overriden from the Makefile
#ifndef TETRIS_DATA_DIR
#define TETRIS_DATA_DIR "."
#endif

#define TETRIS_TOP_SCORE_PATH (TETRIS_DATA_DIR "/top_score.ttrs")

int load_top_score(void) {
    FILE* f = fopen(TETRIS_TOP_SCORE_PATH, "r");
    int top_score = 0;

    if (f != NULL) {
        int status = fscanf(f, "tetris_top_score: %d", &top_score);
        if (status != 1 || top_score < 0) {
            top_score = 0;
        }
        fclose(f);
    }

    return top_score;
}

void save_top_score(int top_score) {
    FILE* f = fopen(TETRIS_TOP_SCORE_PATH, "w");
    if (f == NULL) {
        return;
    }

    // deliberately ignoring errors here
    fprintf(f, "tetris_top_score: %d\n", top_score);
    fclose(f);
}
