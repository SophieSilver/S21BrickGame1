#include <check.h>

#include "../game.h"
#include "tests.h"

START_TEST(moving_down_works) {
    for (int i = 0; i < PIECE_TYPE_COUNT; i += 1) {
        GameData game_data = {
            .state = SPAWN,
            .next_piece = i,
            .top_score_loaded = true,
        };
        while (game_data.state == SPAWN) {
            update_game_data(&game_data);
        }
        ck_assert_int_eq(game_data.state, MOVING);

        for (int j = 0; j < 5; j += 1) {
            int previous_y_position = game_data.played_piece.y;
            game_data.inputs.down = true;
            update_game_data(&game_data);

            ck_assert_int_eq(game_data.played_piece.y, previous_y_position + 1);
        }
    }
}
END_TEST

START_TEST(moving_left_works) {
    for (int i = 0; i < PIECE_TYPE_COUNT; i += 1) {
        GameData game_data = {
            .state = SPAWN,
            .next_piece = i,
            .top_score_loaded = true,
        };
        while (game_data.state == SPAWN) {
            update_game_data(&game_data);
        }
        ck_assert_int_eq(game_data.state, MOVING);

        for (int j = 0; j < 3; j += 1) {
            int previous_x_position = game_data.played_piece.x;
            game_data.inputs.left = true;
            update_game_data(&game_data);

            ck_assert_int_eq(game_data.played_piece.x, previous_x_position - 1);
        }
    }
}
END_TEST

START_TEST(moving_right_works) {
    for (int i = 0; i < PIECE_TYPE_COUNT; i += 1) {
        GameData game_data = {
            .state = SPAWN,
            .next_piece = i,
            .top_score_loaded = true,
        };
        while (game_data.state == SPAWN) {
            update_game_data(&game_data);
        }
        ck_assert_int_eq(game_data.state, MOVING);

        for (int j = 0; j < 3; j += 1) {
            int previous_x_position = game_data.played_piece.x;
            game_data.inputs.right = true;
            update_game_data(&game_data);

            ck_assert_int_eq(game_data.played_piece.x, previous_x_position + 1);
        }
    }
}
END_TEST

START_TEST(rotating_works) {
    for (int i = 0; i < PIECE_TYPE_COUNT; i += 1) {
        GameData game_data = {
            .state = SPAWN,
            .next_piece = i,
            .top_score_loaded = true,
        };
        while (game_data.state == SPAWN) {
            update_game_data(&game_data);
        }
        ck_assert_int_eq(game_data.state, MOVING);

        for (int j = 0; j < 8; j += 1) {
            int previous_rotation = game_data.played_piece.rotation;
            game_data.inputs.rotate = true;
            update_game_data(&game_data);

            ck_assert_int_eq(game_data.played_piece.rotation, (previous_rotation + 1) % 4);
        }
    }
}
END_TEST

START_TEST(falling_works) {
    for (int i = 0; i < PIECE_TYPE_COUNT; i += 1) {
        GameData game_data = {
            .state = SPAWN,
            .next_piece = i,
            .top_score_loaded = true,
        };
        while (game_data.state == SPAWN) {
            update_game_data(&game_data);
        }
        ck_assert_int_eq(game_data.state, MOVING);

        int previous_y_position = game_data.played_piece.y;
        for (int j = 0; j < 20; j += 1) {
            update_game_data(&game_data);
        }
        ck_assert_int_gt(game_data.played_piece.y, previous_y_position);
    }
}
END_TEST

START_TEST(collision_with_left_wall_works) {
    GameData game_data = {
        .state = SPAWN,
        .next_piece = T_PIECE,
        .top_score_loaded = true,
    };
    while (game_data.state == SPAWN) {
        update_game_data(&game_data);
    }
    ck_assert_int_eq(game_data.state, MOVING);

    for (int i = 0; i < 4; i += 1) {
        int previous_x_position = game_data.played_piece.x;
        game_data.inputs.left = true;
        update_game_data(&game_data);

        ck_assert_int_eq(game_data.played_piece.x, previous_x_position - 1);
    }

    int previous_x_position = game_data.played_piece.x;
    game_data.inputs.left = true;
    update_game_data(&game_data);

    ck_assert_int_eq(game_data.played_piece.x, previous_x_position);
}
END_TEST

START_TEST(collision_with_right_wall_works) {
    GameData game_data = {
        .state = SPAWN,
        .next_piece = T_PIECE,
        .top_score_loaded = true,
    };

    while (game_data.state == SPAWN) {
        update_game_data(&game_data);
    }
    ck_assert_int_eq(game_data.state, MOVING);

    for (int i = 0; i < 3; i += 1) {
        int previous_x_position = game_data.played_piece.x;
        game_data.inputs.right = true;
        update_game_data(&game_data);

        ck_assert_int_eq(game_data.played_piece.x, previous_x_position + 1);
    }

    int previous_x_position = game_data.played_piece.x;
    game_data.inputs.right = true;
    update_game_data(&game_data);

    ck_assert_int_eq(game_data.played_piece.x, previous_x_position);
}
END_TEST

START_TEST(collision_with_field_works) {
    GameData game_data = {
        .state = SPAWN,
        // empty comments for nicer clang-format
        .field = {{
            {0, 0, 0, 0, 0, 0, 0, 1, 1, 1},  //
            {0, 0, 0, 0, 0, 0, 0, 1, 1, 1},  //
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
        }},
        .next_piece = T_PIECE,
        .top_score_loaded = true,
    };

    while (game_data.state == SPAWN) {
        update_game_data(&game_data);
    }
    ck_assert_int_eq(game_data.state, MOVING);

    int previous_x_position = game_data.played_piece.x;
    game_data.inputs.right = true;
    update_game_data(&game_data);
    ck_assert_int_eq(game_data.played_piece.x, previous_x_position);
}

Suite *moving_test_suite(void) {
    Suite *s = suite_create("moving_suite");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, moving_down_works);
    tcase_add_test(tc_core, moving_left_works);
    tcase_add_test(tc_core, moving_right_works);
    tcase_add_test(tc_core, rotating_works);
    tcase_add_test(tc_core, falling_works);
    tcase_add_test(tc_core, collision_with_left_wall_works);
    tcase_add_test(tc_core, collision_with_right_wall_works);
    tcase_add_test(tc_core, collision_with_field_works);

    suite_add_tcase(s, tc_core);
    return s;
}
