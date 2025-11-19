#ifndef SCREEN_H
#define SCREEN_H

#include "shapes.h"

typedef struct Screen {
    Shape **elements;
    int count;
    int capacity;
} Screen;

Screen* screen_create(Screen *next);
void screen_add(Screen *s, Shape *shape);
void screen_render(Screen *s, SDL_Renderer* renderer);
void screen_handle_event(Screen *s, SDL_Event *e);
void screen_destroy(Screen *s);
void screen_clear(Screen *s);

#endif
