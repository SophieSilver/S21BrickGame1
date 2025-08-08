#include "cli.h"

int main(void) {
    CliController controller = cli_controller_create();
    cli_controller_run_main_loop(&controller);
    cli_controller_destroy(controller);
}
