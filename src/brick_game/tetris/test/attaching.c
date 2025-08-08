#include <check.h>
#include <stdbool.h>

#include "../field.h"
#include "../game.h"
#include "tests.h"

START_TEST(attaching_to_floor_works) {
    int expected_field[PLAYFIELD_ROWS][PLAYFIELD_COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},  //
    };

    GameData game_data = {
        .state = MOVING,
        .played_piece = {.piece_type = T_PIECE, .x = 3, .y = 18, .rotation = 2},
        .top_score_loaded = true,
    };

    // must transition to attaching
    game_data.inputs.down = true;
    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, ATTACHING);

    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, SPAWN);

    ck_assert_mem_eq(game_data.field.blocks, expected_field, sizeof(expected_field));
}
END_TEST

START_TEST(attaching_to_field_works) {
    int expected_field[PLAYFIELD_ROWS][PLAYFIELD_COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},  //
    };

    GameData game_data = {
        .state = MOVING,
        .field = {{
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},  //
        }},
        .played_piece = {.piece_type = T_PIECE, .x = 2, .y = 16},
        .top_score_loaded = true,
    };

    // must transition to attaching
    game_data.inputs.down = true;
    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, ATTACHING);

    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, SPAWN);

    ck_assert_mem_eq(game_data.field.blocks, expected_field, sizeof(expected_field));
}
END_TEST

START_TEST(clearing_works) {
    int expected_field[PLAYFIELD_ROWS][PLAYFIELD_COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 1},  //
    };

    GameData game_data = {
        .state = MOVING,
        .field = {{
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 1},  //
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 0, 1, 1, 1, 1},  //
        }},
        .played_piece = {.piece_type = T_PIECE, .x = 3, .y = 17},
        .top_score_loaded = true,
    };

    // must transition to attaching
    game_data.inputs.down = true;
    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, ATTACHING);

    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, CLEARING);

    while (game_data.state == CLEARING) {
        update_game_data(&game_data);
    }

    ck_assert_int_eq(game_data.state, SPAWN);
    ck_assert_mem_eq(game_data.field.blocks, expected_field, sizeof(expected_field));
}
END_TEST

Suite *attaching_test_suite(void) {
    Suite *s = suite_create("attaching_suite");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, attaching_to_floor_works);
    tcase_add_test(tc_core, attaching_to_field_works);
    tcase_add_test(tc_core, clearing_works);

    suite_add_tcase(s, tc_core);
    return s;
}
