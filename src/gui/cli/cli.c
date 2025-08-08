#include "cli.h"

#include <ncurses.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

/// How many milliseconds a frame lasts
#define FRAME_MS ((long)50)

CliController cli_controller_create(void) {
    CliController controller;
    controller.game_info = (GameInfo_t) {0};
    controller.running = true;
    controller.view = cli_view_create();

    srand((unsigned int)time(NULL));

    return controller;
}

void cli_controller_destroy(CliController controller) {
    cli_view_destroy(controller.view);
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
            userInput(Start, false);
        } else if (c == KEY_LEFT) {
            userInput(Left, false);
        } else if (c == KEY_RIGHT) {
            userInput(Right, false);
        } else if (c == KEY_UP) {
            userInput(Up, false);
        } else if (c == KEY_DOWN) {
            userInput(Down, false);
        } else if (c == ' ') {
            userInput(Action, false);
        } else if (c == 'p' || c == 'P') {
            userInput(Pause, false);
        } else if (c == 'q' || c == 'Q') {
            userInput(Terminate, false);

            if (controller->game_info.pause == START_SCREEN) {
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
        controller->game_info = updateCurrentState();
        cli_view_update(&controller->view, controller->game_info);
    }
}
