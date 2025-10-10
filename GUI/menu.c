#include "app.h"
#include <stdlib.h>
#include <string.h>

/* D�claration de la fonction screen_sort_create */
Screen* screen_sort_create(App *app, SortType type);

/* === Callback : clic sur un bouton du menu === */
static void on_menu_click(Button *b, void *userdata) {
    App *app = (App*)userdata;
    if (strcmp(b->base.text, "Tri par selection") == 0)
        app_set_screen(app, screen_sort_create(app, SORT_SELECTION));
    else
        app_set_screen(app, screen_sort_create(app, SORT_MERGE));
}

/* === Cr�ation de l��cran du menu === */
Screen* screen_menu_create(App *app) {
    Screen *screen = screen_create();

    /* Bouton Tri par s�lection */
    Button *b_select = button_create(150, 120, 200, 60, "Tri par selection", app->font);
    b_select->base.bg = (SDL_Color){52, 152, 219, 255}; // bleu
    b_select->base.text_color = (SDL_Color){255,255,255,255};
    b_select->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    b_select->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    b_select->base.userdata = app;
    b_select->on_click = on_menu_click;

    /* Bouton Tri par fusion */
    Button *b_merge = button_create(150, 200, 200, 60, "Tri par fusion", app->font);
    b_merge->base.bg = (SDL_Color){46, 204, 113, 255}; // vert
    b_merge->base.text_color = (SDL_Color){255,255,255,255};
    b_merge->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    b_merge->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    b_merge->base.userdata = app;
    b_merge->on_click = on_menu_click;

    /* Texte du titre */
    Shape *title = shape_create(100, 30, 300, 50, "Choisissez un algorithme de tri", app->bg, app->font);

    /* Ajout des �l�ments � l��cran */
    screen_add(screen, title);
    screen_add(screen, (Shape*)b_select);
    screen_add(screen, (Shape*)b_merge);

    return screen;
}
