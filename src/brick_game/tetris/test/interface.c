#include <check.h>
#include <stdlib.h>

#include "../../brick_game.h"
#include "tests.h"

// the library interface is kinda hard to test due to it using static objects,
// so these tests are very minimal

START_TEST(library_interface_works) {
    userInput(Terminate, false);
    updateCurrentState();

    updateCurrentState();
    userInput(Start, false);
    updateCurrentState();
    userInput(Terminate, false);
    userInput(Left, false);
    userInput(Right, false);
    userInput(Down, false);
    userInput(Action, false);
    userInput(Up, false);
    updateCurrentState();
    userInput(Start, false);
    userInput(Pause, false);
    updateCurrentState();
    userInput(Start, false);
    for (int i = 0; i < 1000; i += 1) {
        updateCurrentState();
        userInput(Down, false);
    }
    GameInfo_t game_info = updateCurrentState();
    ck_assert(game_info.pause == GAME_OVER_SCREEN);
}
END_TEST

// Fuzz (https://en.wikipedia.org/wiki/Fuzzing) the library interface to make
// sure it doesn't crash/segfault/perform any invalid operations best results
// with valgrind/sanitizers
START_TEST(library_interface_fuzz) {
    userInput(Terminate, false);
    updateCurrentState();

    // get deterministic results
    srand(1337);

    for (int i = 0; i < 100000; i += 1) {
        for (UserAction_t action = Start; action <= Action; action += 1) {
            if (rand() % 2 == 0) {
                userInput(action, rand() % 2 == 0);
            }
        }

        updateCurrentState();
    }
}

Suite *library_interface_test_suite(void) {
    Suite *s = suite_create("library_interface_suite");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, library_interface_works);
    tcase_add_test(tc_core, library_interface_fuzz);

    suite_add_tcase(s, tc_core);
    return s;
}
