#ifndef BRICK_GAME_V1_0_BRICK_GAME_TETRIS_UTILS_H_
#define BRICK_GAME_V1_0_BRICK_GAME_TETRIS_UTILS_H_

#include <stdlib.h>

#define MAX(A, B) ((A) >= (B) ? (A) : (B))
#define MIN(A, B) ((A) <= (B) ? (A) : (B))

/// Convert a flat 2d array to a matrix, i.e. an array of pointers to rows
static inline void array2d_to_matrix(int *array, int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i += 1) {
        matrix[i] = &array[(size_t)i * (size_t)cols];
    }
}

#endif  // BRICK_GAME_V1_0_BRICK_GAME_TETRIS_UTILS_H_
