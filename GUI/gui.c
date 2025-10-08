#include "gui.h"
#include "shapes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GUI *gui_create(const char *title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return NULL;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init error: %s\n", TTF_GetError());
        SDL_Quit();
        return NULL;
    }

    GUI *gui = malloc(sizeof(GUI));

    gui->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if (!gui->window) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        free(gui);
        return NULL;
    }

    gui->renderer = SDL_CreateRenderer(gui->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gui->renderer) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(gui->window); free(gui);
        return NULL;
    }

    gui->count = 0;
    gui->capacity = 8;
    gui->items = calloc(gui->capacity, sizeof(Shape*));
    return gui;
}

void gui_destroy(GUI *gui) {
    if (!gui) return;
    for (int i=0;i<gui->count;i++) {
        shape_destroy(gui->items[i]);
    }
    free(gui->items);
    SDL_DestroyRenderer(gui->renderer);
    SDL_DestroyWindow(gui->window);
    TTF_Quit();
    SDL_Quit();
    free(gui);
}

void gui_add(GUI *gui, Shape *s) {
    if (gui->count >= gui->capacity) {
        gui->capacity *= 2;
        gui->items = realloc(gui->items, gui->capacity * sizeof(Shape*));
    }
    gui->items[gui->count++] = s;
}

void gui_remove(GUI *gui, Shape *s) {
    int idx = -1;
    for (int i=0;i<gui->count;i++)
        if (gui->items[i] == s)
        {
            idx = i;
            break;
        }

    if (idx==-1)
        return;

    shape_destroy(gui->items[idx]);
    for (int i=idx;i<gui->count-1;i++)
        gui->items[i]=gui->items[i+1];
    gui->count--;
}

/* Dispatch mouse/keyboard events to each shape (top-down) */
void gui_handle_event(GUI *gui, SDL_Event *ev) {
    for (int i=0;i<gui->count;i++) {
        Shape *s = gui->items[i];
        if (s->handle_event)
            s->handle_event(s, ev);
    }
}

void gui_render(GUI *gui) {
    SDL_SetRenderDrawColor(gui->renderer, 240,240,240,255);
    SDL_RenderClear(gui->renderer);

    for (int i=0;i<gui->count;i++) {
        Shape *s = gui->items[i];
        if (s->draw)
            s->draw(gui->renderer, s);
    }

    SDL_RenderPresent(gui->renderer);
}

/* optional per-frame updates */
void gui_update(GUI *gui, float dt) {
    for (int i=0;i<gui->count;i++) {
        Shape *s = gui->items[i];
        if (s->update)
            s->update(s, dt);
    }
}

