#ifndef APP_H
#define APP_H

static int INPUT_ACTIVE_COUNT = 0;
#include "screen.h"

typedef struct App {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    Screen *screen;
    SDL_Color bg;
} App;

App* app_create(const char *title, int w, int h);
void app_set_screen(App *app, Screen *screen);
int app_run(App *app);
void app_destroy(App *app);

#endif
