#include "app.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* D�claration de la fonction screen_menu_create */
Screen* screen_menu_create(App *app);

/* === Callback : bouton Retour === */
static void on_back(Button *b, void *userdata) {
    App *app = (App*)userdata;
    app_set_screen(app, screen_menu_create(app));
}

/* === Callback : bouton Trier === */
static void on_sort(Button *b, void *userdata) {
    Screen *screen = (Screen*)userdata;

    InputField *input = NULL;
    Shape *output = NULL;
    SortType *type_ptr = NULL;

    /* R�cup�re les pointeurs utiles stock�s dans userdata des formes */
    for (int i = 0; i < screen->count; i++) {
        Shape *s = screen->elements[i];
        if (!s) continue;
        if (s->userdata && strcmp((char*)s->userdata, "input") == 0)
            input = (InputField*)s;
        else if (s->userdata && strcmp((char*)s->userdata, "output") == 0)
            output = s;
    }

    if (!input || !output) return;

    /* Parse les nombres */
    int arr[128];
    int n = 0;
    char temp[256];
    strcpy(temp, input->buffer);
    char *tok = strtok(temp, " ,-;+");
    while (tok && n < 128) {
        arr[n++] = atoi(tok);
        tok = strtok(NULL, " ,;+");
    }

    /* Ex�cute le tri selon le type */
    if (CURRENT_SORT_TYPE == SORT_SELECTION)
        selection_sort(arr, n);
    else
        merge_sort(arr, n);

    /* Construit la chaîne de sortie */
    char buf[512] = "";
    for (int i = 0; i < n; i++) {
        char tmp[16];
        sprintf(tmp, "%d  ", arr[i]);
        strcat(buf, tmp);
    }

    free(output->text);
    output->text = strdup(buf);
}

/* === Création de l'écran du tri === */
Screen* screen_sort_create(App *app, SortType type) {
    Screen *screen = screen_create();
    CURRENT_SORT_TYPE = type;

    /* Titre */
    const char *algo_name = (type == SORT_SELECTION) ? "Tri par selection" : "Tri par fusion";
    Shape *title = shape_create(300, 40, 200, 50, algo_name, app->bg, app->font);

    /* Champ de saisie */
    InputField *input = inputfield_create(200, 150, 400, 40, 30, "5,8,300,-4,15", app->font);
    input->base.bg = (SDL_Color){255,255,255,255};
    input->base.border = (SDL_Color){100,100,100,255};
    input->base.border_width = 1;
    input->base.text_color = (SDL_Color){0,0,0,255};
    input->color_focused = (SDL_Color){200, 200, 150, 255}; // lavande
    input->base.userdata = "input";

    /* Zone de texte pour le résultat */
    Shape *output = shape_create(200, 300, 400, 40, "", (SDL_Color){0,0,0,160}, app->font);
    output->bg = (SDL_Color){255,255,255,255};
    output->border = (SDL_Color){100,100,100,255};
    output->border_width = 1;
    output->userdata = "output";

    /* Bouton Trier */
    Button *btn_sort = button_create(200, 350, 120, 50, "Trier", app->font);
    btn_sort->base.bg = (SDL_Color){155, 89, 182, 255}; // violet
    btn_sort->base.text_color = (SDL_Color){255,255,255,255};
    btn_sort->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    btn_sort->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    btn_sort->base.userdata = screen; // passe l'écran en userdata
    btn_sort->on_click = on_sort;

    /* Bouton Retour */
    Button *btn_back = button_create(480, 350, 120, 50, "Retour", app->font);
    btn_back->base.bg = (SDL_Color){231, 76, 60, 255}; // rouge
    btn_back->base.text_color = (SDL_Color){255,255,255,255};
    btn_back->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    btn_back->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    btn_back->on_click = on_back;
    btn_back->base.userdata = app;

    /* Ajout des éléments */
    screen_add(screen, title);
    screen_add(screen, (Shape*)input);
    screen_add(screen, output);
    screen_add(screen, (Shape*)btn_sort);
    screen_add(screen, (Shape*)btn_back);

    return screen;
}
