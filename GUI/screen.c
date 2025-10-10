#include "screen.h"
#include <stdlib.h>
#include <stdio.h>

Screen* screen_create(void) {
    Screen *s = malloc(sizeof(Screen));
    s->capacity = 8;
    s->elements = malloc(s->capacity * sizeof(Shape*));
    return s;
}

void screen_add(Screen *s, Shape *shape) {
    if (s->count >= s->capacity) {
        s->capacity *= 2;
        s->elements = realloc(s->elements, s->capacity * sizeof(Shape*));
    }
    s->elements[s->count++] = shape;
}

/* === Rendu de tous les éléments === */
void screen_render(Screen *s, SDL_Renderer *renderer) {
    for (int i = 0; i < s->count; i++) {
        Shape *sh = s->elements[i];
        if (sh && sh->visible && sh->draw) {
            sh->draw(sh, renderer);
        }
    }
}

/* === Gestion des événements (clics, clavier, etc.) === */
void screen_handle_event(Screen *s, SDL_Event *e) {
    for (int i = 0; i < s->count; i++) {
        Shape *sh = s->elements[i];
        if (sh && sh->handle_event) {
            sh->handle_event(sh, e);
        }
    }
}

/* === Libération complète d’un écran === */
void screen_destroy(Screen *s) {
    if (!s) return;
    for (int i = 0; i < s->count; i++) {
        if (s->elements[i] && s->elements[i]->destroy) {
            s->elements[i]->destroy(s->elements[i]);
        }
    }
    free(s->elements);
    free(s);
}
