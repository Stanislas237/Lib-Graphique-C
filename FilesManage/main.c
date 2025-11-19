#define SDL_MAIN_HANDLED
#include "app.h"

int main() {
    App *app = app_create("Implementation d'une file", 500, 300);

    int quit = 0;
    while (quit == 0)
        quit = app_run(app);
    app_destroy(app);
    return 0;
}

