#include <check.h>

#include "../game.h"
#include "../game/saving.h"
#include "tests.h"

START_TEST(pausing_works) {
    GameData game_data = {
        .state = SPAWN,
        .next_piece = T_PIECE,
        .top_score_loaded = true,
    };

    while (game_data.state == SPAWN) {
        update_game_data(&game_data);
    }
    GameState state = game_data.state;
    int timer = game_data.timer;
    game_data.inputs.pause = true;
    update_game_data(&game_data);

    for (int i = 0; i < 1000; i += 1) {
        update_game_data(&game_data);
        ck_assert(game_data.paused);
        ck_assert_int_eq(timer, game_data.timer);
        ck_assert_int_eq(state, game_data.state);
    }

    game_data.inputs.pause = true;
    update_game_data(&game_data);
    ck_assert(!game_data.paused);
}
END_TEST

START_TEST(saving_works) {
    const int saved_score = 1234567;

    save_top_score(saved_score);
    int loaded_score = load_top_score();
    ck_assert_int_eq(saved_score, loaded_score);

    GameData game_data = {0};
    update_game_data(&game_data);
    ck_assert(game_data.top_score_loaded);
    ck_assert_int_eq(game_data.top_score, saved_score);
}
END_TEST

Suite *misc_test_suite(void) {
    Suite *s = suite_create("misc_suite");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, pausing_works);
    tcase_add_test(tc_core, saving_works);

    suite_add_tcase(s, tc_core);
    return s;
}
