#include <check.h>

#include "../game.h"
#include "../game/common.h"
#include "tests.h"

START_TEST(start_works) {
    GameData game_data = {.top_score_loaded = true};
    game_data.state = START;
    // stays in start
    for (int i = 0; i < 100; i += 1) {
        update_game_data(&game_data);
        ck_assert_int_eq(game_data.state, START);
    }

    // transitions to spawn_start
    game_data.inputs.start = true;
    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, SPAWN_START);
}
END_TEST

START_TEST(terminate_works) {
    GameData game_data = {.top_score_loaded = true, .state = MOVING};
    update_game_data(&game_data);
    game_data.inputs.terminate = true;
    update_game_data(&game_data);

    ck_assert_int_eq(game_data.state, START);
}
END_TEST

START_TEST(spawn_start_works) {
    for (int i = 0; i < 100; i += 1) {
        GameData game_data = {.top_score_loaded = true};
        game_data.state = START;
        update_game_data(&game_data);
        transition_state(&game_data, SPAWN_START);
        update_game_data(&game_data);

        ck_assert_int_eq(game_data.state, MOVING);
        ck_assert_int_eq(game_data.played_piece.x, 3);
        if (game_data.played_piece.piece_type == I_PIECE) {
            ck_assert_int_eq(game_data.played_piece.y, -2);
        } else {
            ck_assert_int_eq(game_data.played_piece.y, -1);
        }
    }
}
END_TEST

START_TEST(game_over_works) {
    GameData game_data = {
        .state = SPAWN,
        .field = {{
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  //
        }},
        .next_piece = T_PIECE,
        .top_score_loaded = true,
    };

    while (game_data.state == SPAWN) {
        update_game_data(&game_data);
    }

    ck_assert_int_eq(game_data.state, GAME_OVER);
    for (int i = 0; i < 100; i += 1) {
        update_game_data(&game_data);
        ck_assert_int_eq(game_data.state, GAME_OVER);
    }

    game_data.inputs.start = true;
    update_game_data(&game_data);
    ck_assert_int_eq(game_data.state, SPAWN_START);
}

Suite *start_test_suite(void) {
    Suite *s = suite_create("start_suite");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, start_works);
    tcase_add_test(tc_core, terminate_works);
    tcase_add_test(tc_core, spawn_start_works);
    tcase_add_test(tc_core, game_over_works);

    suite_add_tcase(s, tc_core);
    return s;
}
