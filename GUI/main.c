#define SDL_MAIN_HANDLED

#include <stdio.h>
#include "app.h"

int main(void) {
    App *app = app_create("Algorithmes de tri", 800, 600);
    int running = 1;
    SDL_Event *ev;

    while (running)
        running = app_run(app, ev);
    app_destroy(app);
    return 0;
}

