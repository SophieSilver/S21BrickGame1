#include "view.h"

#include <ncurses.h>

#define FIELD_WIN_ROWS (FIELD_ROWS + 2)
#define FIELD_WIN_COLS (FIELD_COLS * 2 + 4)
#define FIELD_WIN_Y 0
#define FIELD_WIN_X 5

#define SIDE_MARGIN 5
#define SIDE_WIN_X (FIELD_WIN_X + FIELD_WIN_COLS + SIDE_MARGIN)
#define SIDE_WIN_ROWS (FIELD_WIN_ROWS)
#define SIDE_BOX_COLS (NEXT_COLS * 2 + 4)
#define SIDE_WIN_COLS (SIDE_BOX_COLS * 2 + SIDE_MARGIN)
#define SIDE_WIN_Y 0

#define TOP_SCORE_WIN_Y 0
#define TOP_SCORE_WIN_ROWS 5

#define SCORE_WIN_Y (TOP_SCORE_WIN_Y + TOP_SCORE_WIN_ROWS - 1)
#define SCORE_WIN_ROWS 5

#define NEXT_WIN_Y (SCORE_WIN_Y + SCORE_WIN_ROWS + 1)
#define NEXT_WIN_ROWS (NEXT_ROWS + 2)

#define LEVEL_WIN_Y (NEXT_WIN_Y + NEXT_WIN_ROWS + 1)
#define LEVEL_WIN_ROWS 5

#define SPEED_WIN_X (SIDE_BOX_COLS + SIDE_MARGIN)
#define SPEED_WIN_Y LEVEL_WIN_Y
#define SPEED_WIN_ROWS LEVEL_WIN_ROWS

#define START_WIN_Y 7
#define START_WIN_X 3
#define START_WIN_ROWS 6
#define START_WIN_COLS (FIELD_WIN_COLS - 6)

#define GAME_OVER_WIN_Y 6
#define GAME_OVER_WIN_X 3
#define GAME_OVER_WIN_ROWS 8
#define GAME_OVER_WIN_COLS (FIELD_WIN_COLS - 6)

#define PAUSE_WIN_Y 7
#define PAUSE_WIN_X 6
#define PAUSE_WIN_ROWS 5
#define PAUSE_WIN_COLS (FIELD_WIN_COLS - 12)

#define CONTROLS_ROWS 24
#define CONTROLS_COLS 20
#define CONTROLS_Y 0
#define CONTROLS_X (SIDE_WIN_X + SIDE_BOX_COLS + SIDE_MARGIN)

static inline void init_windows(CliView *view) {
    view->field_win = subwin(stdscr, FIELD_WIN_ROWS, FIELD_WIN_COLS, FIELD_WIN_Y, FIELD_WIN_X);
    view->start_win = derwin(view->field_win, START_WIN_ROWS, START_WIN_COLS, START_WIN_Y, START_WIN_X);
    view->game_over_win =
        derwin(view->field_win, GAME_OVER_WIN_ROWS, GAME_OVER_WIN_COLS, GAME_OVER_WIN_Y, GAME_OVER_WIN_X);
    view->pause_win = derwin(view->field_win, PAUSE_WIN_ROWS, PAUSE_WIN_COLS, PAUSE_WIN_Y, PAUSE_WIN_X);

    view->side_win = subwin(stdscr, SIDE_WIN_ROWS, SIDE_WIN_COLS, SIDE_WIN_Y, SIDE_WIN_X);
    view->top_score_win = derwin(view->side_win, TOP_SCORE_WIN_ROWS, SIDE_BOX_COLS, TOP_SCORE_WIN_Y, 0);
    view->score_win = derwin(view->side_win, SCORE_WIN_ROWS, SIDE_BOX_COLS, SCORE_WIN_Y, 0);
    view->next_win = derwin(view->side_win, NEXT_WIN_ROWS, SIDE_BOX_COLS, NEXT_WIN_Y, 0);
    view->level_win = derwin(view->side_win, LEVEL_WIN_ROWS, SIDE_BOX_COLS, LEVEL_WIN_Y, 0);
    view->speed_win = derwin(view->side_win, SPEED_WIN_ROWS, SIDE_BOX_COLS, SPEED_WIN_Y, SPEED_WIN_X);

    view->controls_win = subwin(stdscr, CONTROLS_ROWS, CONTROLS_COLS, CONTROLS_Y, CONTROLS_X);
}

CliView cli_view_create(void) {
    initscr();
    // PRESS START FOR NEW GAME
    // don't block on getch
    nodelay(stdscr, true);
    // allow capturing KEY_LEFT and the like
    keypad(stdscr, true);
    // don;t buffer lines
    cbreak();
    // don't print pressed characters
    noecho();
    // don't show cursor
    curs_set(0);

    CliView view = {0};
    view.frame_count = 0;
    init_windows(&view);

    return view;
}

void cli_view_destroy(CliView view) {
    delwin(view.next_win);
    delwin(view.score_win);
    delwin(view.top_score_win);
    delwin(view.level_win);
    delwin(view.side_win);

    delwin(view.start_win);
    delwin(view.game_over_win);
    delwin(view.pause_win);
    delwin(view.field_win);

    delwin(view.controls_win);
    endwin();
}

static inline void draw_field_decorations(CliView *view) {
    for (int i = 0; i < FIELD_ROWS; i += 1) {
        wmove(view->field_win, i, 0);

        wprintw(view->field_win, "<!");
        for (int j = 0; j < FIELD_COLS; j += 1) {
            wprintw(view->field_win, " .");
        }
        wprintw(view->field_win, "!>");
    }

    wmove(view->field_win, FIELD_ROWS, 0);
    wprintw(view->field_win, "<!");
    for (int i = 0; i < FIELD_COLS; i += 1) {
        wprintw(view->field_win, "==");
    }
    wprintw(view->field_win, "!>");

    wmove(view->field_win, FIELD_ROWS + 1, 2);
    for (int i = 0; i < FIELD_COLS; i += 1) {
        wprintw(view->field_win, "\\/");
    }
}

static inline void draw_border(WINDOW *win) {
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
}

static inline void draw_blocks(WINDOW *win, int **blocks, int rows, int cols, int y, int x) {
    for (int i = 0; i < rows; i += 1) {
        for (int j = 0; j < cols; j += 1) {
            if (blocks[i][j]) {
                mvwprintw(win, y + i, x + 2 * j, "[]");
            }
        }
    }
}

static inline void draw_field_blocks(CliView *view, int **field) {
    draw_blocks(view->field_win, field, FIELD_ROWS, FIELD_COLS, 0, 2);
}

static inline void draw_next_win(CliView *view, int **next) {
    draw_border(view->next_win);
    mvwprintw(view->next_win, 0, 4, "NEXT");

    draw_blocks(view->next_win, next, NEXT_ROWS, NEXT_COLS, 1, 2);
}

static inline void draw_top_score_win(CliView *view, int top_score) {
    draw_border(view->top_score_win);
    mvwprintw(view->top_score_win, 0, 3, "TOP");
    mvwprintw(view->top_score_win, 2, 3, "%06d", top_score);
}

static inline void draw_score_win(CliView *view, int score) {
    draw_border(view->score_win);
    mvwprintw(view->score_win, 0, 3, "SCORE");
    mvwprintw(view->score_win, 2, 3, "%06d", score);
}

static inline void draw_level_win(CliView *view, int level) {
    draw_border(view->level_win);
    mvwprintw(view->level_win, 0, 3, "LEVEL");
    mvwprintw(view->level_win, 2, 7, "%02d", level);
}

static inline void draw_speed_win(CliView *view, int speed) {
    draw_border(view->speed_win);
    mvwprintw(view->speed_win, 0, 3, "SPEED");
    mvwprintw(view->speed_win, 2, 7, "%02d", speed);
}

static inline void draw_side_win(CliView *view, GameInfo game_info) {
    draw_top_score_win(view, game_info.high_score);

    if (game_info.status != GAME_STATUS_START) {
        draw_score_win(view, game_info.score);
    }
    if (game_info.status == GAME_STATUS_RUNNING) {
        draw_next_win(view, game_info.next);
    }
    if (game_info.status != GAME_STATUS_START) {
        draw_level_win(view, game_info.level);
        draw_speed_win(view, game_info.speed);
    }
}

static inline void draw_menu_screen(const CliView *view, WINDOW *window, char *contents[], int contents_len) {
    werase(window);
    draw_border(window);

    if (view->frame_count % 18 < 10) {
        for (int i = 0; i < contents_len; i += 1) {
            mvwprintw(window, 2 + i, 3, "%s", contents[i]);
        }
    }
}

static inline void draw_start_screen(CliView *view) {
    draw_menu_screen(view, view->start_win, (char *[]) {"PRESS  START", "FOR NEW GAME"}, 2);
}

static inline void draw_game_over_screen(CliView *view) {
    draw_menu_screen(view, view->game_over_win, (char *[]) {" GAME  OVER ", "", "PRESS  START", "FOR NEW GAME"}, 4);
}

static inline void draw_pause_screen(CliView *view) {
    draw_menu_screen(view, view->pause_win, (char *[]) {"PAUSED"}, 1);
}

static inline void draw_menu(CliView *view, GameInfo game_info) {
    switch (game_info.status) {
        case GAME_STATUS_RUNNING:
            return;

        case GAME_STATUS_START:
            draw_start_screen(view);
            break;

        case GAME_STATUS_GAME_OVER:
            draw_game_over_screen(view);
            break;

        case GAME_STATUS_PAUSED:
            draw_pause_screen(view);
            break;
            ;
    }
}

static inline void draw_controls(CliView *view) {
    wmove(view->controls_win, 0, 0);
    wprintw(
        view->controls_win,
        "START     [Enter]\n"
        "PAUSE     [P]\n"
        "END GAME  [Q]\n"
        "ACTION    [Space]\n"
        "UP        [U.Arrow]\n"
        "DOWN      [D.Arrow]\n"
        "LEFT      [L.Arrow]\n"
        "RIGHT     [R.Arrow]\n"
    );
}

static inline void draw_view(CliView *view, GameInfo game_info) {
    draw_field_decorations(view);
    if (game_info.status == GAME_STATUS_RUNNING || game_info.status == GAME_STATUS_GAME_OVER) {
        draw_field_blocks(view, game_info.field);
    }

    draw_menu(view, game_info);

    draw_side_win(view, game_info);
    draw_controls(view);
}

void cli_view_update(CliView *view, GameInfo game_info) {
    erase();

    draw_view(view, game_info);

    // docs say to touch parent windows before refreshing subwindows
    touchwin(stdscr);
    refresh();
    view->frame_count += 1;
}
