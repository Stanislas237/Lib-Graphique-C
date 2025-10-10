#include "screen.h"
#include <stdlib.h>
#include <stdio.h>

Screen* screen_create(Screen *next) {
    Screen *s = malloc(sizeof(Screen));
    s->count = 0;
    s->capacity = 5;
    s->elements = malloc(s->capacity * sizeof(Shape*));
    s->next = next;
    return s;
}

void screen_add(Screen *s, Shape *shape) {
    if (s->count >= s->capacity) {
        s->capacity += s->capacity;
        s->elements = realloc(s->elements, s->capacity * sizeof(Shape*));
    }
    s->elements[s->count++] = shape;
}

/* === Rendu de tous les �l�ments === */
void screen_render(Screen *s, SDL_Renderer *renderer) {
    for (int i = 0; i < s->count; i++) {
        Shape *sh = s->elements[i];
        if (sh && sh->visible && sh->draw)
            sh->draw(renderer, sh);
    }
}

/* === Gestion des �v�nements (clics, clavier, etc.) === */
void screen_handle_event(Screen *s, SDL_Event *e) {
    for (int i = 0; i < s->count; i++) {
        Shape *sh = s->elements[i];
        if (sh && sh->handle_event) {
            sh->handle_event(sh, e);
        }
    }
}

/* === Lib�ration compl�te d�un �cran === */
void screen_destroy(Screen *s) {
    if (!s) return;
    for (int i = 0; i < s->count; i++)
        if (s->elements[i] && s->elements[i]->destroy)
            s->elements[i]->destroy(s->elements[i]);

    free(s->elements);
    free(s);
}

/* === Reset de tous les boutons === */
void screen_clear(Screen *s){
    if (!s) return;
    for (int i = 0; i < s->count; i++)
        if (s->elements[i] && s->elements[i]->reset)
            s->elements[i]->reset(s->elements[i]);
}
