#include <check.h>
#include <stdlib.h>

#include "../../brick_game.h"
#include "tests.h"

START_TEST(library_interface_works) {
    GameInstance *instance = game_instance_create();
    user_input(instance, Terminate);
    update_current_state(instance);

    update_current_state(instance);
    user_input(instance, Start);
    update_current_state(instance);
    user_input(instance, Terminate);
    user_input(instance, Left);
    user_input(instance, Right);
    user_input(instance, Down);
    user_input(instance, Action);
    user_input(instance, Up);
    update_current_state(instance);
    user_input(instance, Start);
    user_input(instance, Pause);
    update_current_state(instance);
    user_input(instance, Start);
    for (int i = 0; i < 1000; i += 1) {
        update_current_state(instance);
        user_input(instance, Down);
    }

    GameInfo game_info = update_current_state(instance);
    game_instance_destroy(instance);

    ck_assert(game_info.status == GAME_STATUS_GAME_OVER);
}
END_TEST

// Fuzz (https://en.wikipedia.org/wiki/Fuzzing) the library interface to make
// sure it doesn't crash/segfault/perform any invalid operations
// best results with valgrind/sanitizers
START_TEST(library_interface_fuzz) {
    GameInstance *instance = game_instance_create();
    user_input(instance, Terminate);
    update_current_state(instance);

    // get deterministic results
    srand(1337);

    for (int i = 0; i < 10000; i += 1) {
        for (UserAction action = Start; action <= Action; action += 1) {
            if (rand() % 2 == 0) {
                user_input(instance, action);
            }
        }

        update_current_state(instance);
    }

    game_instance_destroy(instance);
}

Suite *library_interface_test_suite(void) {
    Suite *s = suite_create("library_interface_suite");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, library_interface_works);
    tcase_add_test(tc_core, library_interface_fuzz);

    suite_add_tcase(s, tc_core);
    return s;
}
