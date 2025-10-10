#ifndef APP_H
#define APP_H

#include "utils.h"
#include "screen.h"

typedef struct App {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    Screen *screen;
} App;

App* app_create(const char *title, int w, int h);
void app_set_screen(App *app, Screen *screen);
int app_run(App *app, SDL_Event *e);
void app_destroy(App *app);

#endif
