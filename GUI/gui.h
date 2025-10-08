#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

typedef struct Shape Shape;

/* GUI manager */
typedef struct GUI {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Shape **items;
    int count;
    int capacity;
} GUI;

GUI *gui_create(const char *title, int w, int h);
void gui_destroy(GUI *gui);
void gui_add(GUI *gui, Shape *s);
void gui_remove(GUI *gui, Shape *s);
void gui_handle_event(GUI *gui, SDL_Event *ev);
void gui_render(GUI *gui);
void gui_update(GUI *gui, float dt); /* optional per-frame updates */

#endif // GUI_H

