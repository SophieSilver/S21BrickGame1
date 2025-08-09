#include "cli.h"

#include <ncurses.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#include "view.h"

/// How many milliseconds a frame lasts
#define FRAME_MS ((long)50)

CliController cli_controller_create(void) {
    CliController controller = {
        .game_info = (GameInfo) {0},
        .game_instance = game_instance_create(),
        .running = true,
        .view = cli_view_create(),
    };

    srand((unsigned int)time(NULL));

    return controller;
}

void cli_controller_destroy(CliController controller) {
    cli_view_destroy(controller.view);
    game_instance_destroy(controller.game_instance);
}

static inline void wait_for_frame(void) {
    struct timespec t = {
        .tv_sec = 0,
        .tv_nsec = FRAME_MS * 1000000,
    };
    thrd_sleep(&t, NULL);
}

static inline void get_inputs(CliController *controller) {
    int c = getch();

    while (c != ERR) {
        if (c == '\n') {
            user_input(controller->game_instance, Start);
        } else if (c == KEY_LEFT) {
            user_input(controller->game_instance, Left);
        } else if (c == KEY_RIGHT) {
            user_input(controller->game_instance, Right);
        } else if (c == KEY_UP) {
            user_input(controller->game_instance, Up);
        } else if (c == KEY_DOWN) {
            user_input(controller->game_instance, Down);
        } else if (c == ' ') {
            user_input(controller->game_instance, Action);
        } else if (c == 'p' || c == 'P') {
            user_input(controller->game_instance, Pause);
        } else if (c == 'q' || c == 'Q') {
            user_input(controller->game_instance, Terminate);

            if (controller->game_info.status == GAME_STATUS_START) {
                controller->running = false;
            }
        }

        c = getch();
    }
}

void cli_controller_run_main_loop(CliController *controller) {
    while (controller->running) {
        wait_for_frame();
        get_inputs(controller);
        controller->game_info = update_current_state(controller->game_instance);
        cli_view_update(&controller->view, controller->game_info);
    }
}
